#include "infrared.h"

#include "buffer.h"
#include "logger.h"

// In the IR protocol every "thing" we send is not called a packet, but
// rather a "frame". There are two types of frames: payload frames and
// acknowledgement frames. The payload frame consists out of the frame
// parts: TYPE, ID, PAYLOAD, PAYLOAD and PARITY. The acknowledge frame
// consists only out of the TYPE and ID parts.

// Each part has defines describing how many bits wide said part is,
// and a bitmask that can be used to extract those bits.

// The PARITY part of a payload frame.
#define FRAME_PARITY_BITS 7
#define FRAME_PARITY_MASK 0b1111111
#define FRAME_PARITY_DEFAULT 73

// The PAYLOAD part of a payload frame.
#define FRAME_PAYLOAD_BITS 7
#define FRAME_PAYLOAD_MASK 0b1111111
#define FRAME_PAYLOAD_LENGTH 2

// The ID part of a payload or acknowledgement frame.
#define FRAME_ID_BITS 3
#define FRAME_ID_MASK 0b111
#define FRAME_ID_MODULO (FRAME_ID_MASK + 1)

// The TYPE part of a payload or acknowledgement frame.
#define FRAME_TYPE_BITS 1
#define FRAME_TYPE_MASK 0b1

// The possible values for the TYPE part of a frame.
#define FRAME_TYPE_VALUE_ACKNOWLEDGE 0b1
#define FRAME_TYPE_VALUE_PAYLOAD 0b0

// The amount of bits used by acknowledgement or payload frames.
#define FRAME_BITS_ACKNOWLEDGE (FRAME_TYPE_BITS + FRAME_ID_BITS)
#define FRAME_BITS_PAYLOAD (FRAME_TYPE_BITS + FRAME_ID_BITS + \
    (FRAME_PAYLOAD_BITS * FRAME_PAYLOAD_LENGTH) + FRAME_PARITY_BITS)

buffer_t inbound;
buffer_t outbound;
uint32_t network_time = 0;

// Receiving: The ID of the last received frame.
uint8_t recv_last_id = 0;

// Receiving: The bits we are receiving and how many there are.
volatile uint32_t recv_data = 0;
volatile uint8_t recv_data_bits = 0;

// Receiving: The last finished frame we received that is ready to be processed.
volatile uint32_t recv_cache = 0;
volatile bool recv_cache_ready = false;

// Receiving: How long the current signal has lasted for.
volatile uint8_t recv_signal_length = 0;

// Transmitting: The ID we will be sending in the next frame.
uint8_t send_id = 1;

// Transmitting: If we should acknowledge a received packet.
bool send_should_acknowledge = false;

// Transmitting: If our frame has been acknowledged and when we last sent it.
bool send_is_acknowledged = true;
uint16_t send_time = 0;

// Transmitting: A cache of the last transmitted frame to resend when
//               the receiver has not acknowledged our transmission in time.
uint32_t send_cache_data = 0;
uint8_t send_cache_data_bits = 0;

// Transmitting: The bits we are transmitting and how many there are.
volatile uint32_t send_data = 0;
volatile uint8_t send_data_bits = 0;

// Transmitting: The current signal and how much longer it lasts.
volatile uint8_t send_signal = 0;
volatile uint8_t send_signal_length = 0;

void ir_init() {
    buffer_init(&inbound);
    buffer_init(&outbound);

    // Prepare Timer0.
    // Mode: Normal.
    // Prescaler: 1/64 (CS01, CS00).
    // Interrupt: Overflow (TOIE0), Compare A (OCIE0A).
    TCCR0A = 0;
    TCCR0B = (1 << CS01) | (1 << CS00);
    // COMPA: Halfway through the cycle
    // OVF: At the end of the cycle.
    OCR0A = 128;
    OCR0B = 0;

    // Prepare Timer2.
    // Mode: Fast PWM (TOP=OCRA, TOV=TOP) (WGM20, WGM21, WGM22).
    // Prescaler: 1/8 (CS21).
    // OC2B: Clear on compare match (COM2B1).
    TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
    TCCR2B = (1 << CS21) | (1 << WGM22);

    // Configure the RX and TX as INPUT.
    // ir_transmit_high() sets TX as OUTPUT.
    DDRD &= ~(1 << PORTD2); // RX, Arduino pin 2.
    DDRD &= ~(1 << PORTD3); // TX, Arduino pin 3.

    // Arbitrary value for the duty cycle of the square wave produced by Timer2.
    // This is known to work with the OCRA value of either 56KHz or the 38KHz modes.
    // For this reason: DO NOT CHANGE UNLESS NECESSARY!
    OCR2B = 20;

    // Detect both rising and falling edges on INT0/PD2 (Arduino pin 2).
    EICRA |= (1 << ISC00);
}

void ir_freq(ir_freq_t freq) {
    // Set the TOP of Timer2 based on the desired frequency.
    OCR2A = freq;

    // Clear the Timer2 counter.
    TCNT2 = 0;
}

inline void ir_transmit_low() {
    // Disable the TX pin.
    DDRD &= ~(1 << PORTD3);
}

inline void ir_transmit_high() {
    // Enable the TX pin.
    DDRD |= (1 << PORTD3);
}

inline bool ir_get_signal() {
    // Read the RX pin. Inverted since the receiver module gives out an
    // inverted signal of the IR signal it is receiving.
    return !(PIND & (1 << PIND2));
}

void ir_enable() {
    // Enable Timer0 interrupts.
    TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);

    // Enable the pin change interrupt.
    EIMSK |= (1 << INT0);
}

void ir_disable() {
    // Disable the IR LED.
    ir_transmit_low();

    // Disable Timer0 interrupts.
    TIMSK0 &= ~((1 << OCIE0A) | (1 << OCIE0B));

    // Disable the pin change interrupt.
    EIMSK &= ~(1 << INT0);
}

uint32_t frame_encode(uint8_t type, uint8_t id, uint8_t *payload) {
    uint32_t frame = 0;

    // Encode the payload while calculating the parity.
    if (payload) {
        uint8_t parity = FRAME_PARITY_DEFAULT;
        for (int i = 0; i < FRAME_PAYLOAD_LENGTH; i++) {
            // Update the parity.
            uint8_t byte = payload[i] & FRAME_PAYLOAD_MASK;
            parity ^= byte;

            // Encode the payload byte.
            frame <<= FRAME_PAYLOAD_BITS;
            frame |= byte;
        }

        // Encode the parity.
        frame <<= FRAME_PARITY_BITS;
        frame |= parity & FRAME_PARITY_MASK;
    }

    // Encode the identifier.
    frame <<= FRAME_ID_BITS;
    frame |= id & FRAME_ID_MASK;

    // Encode the type.
    frame <<= FRAME_TYPE_BITS;
    frame |= type & FRAME_TYPE_MASK;

    return frame;
}

bool frame_decode(uint32_t frame, uint8_t *type, uint8_t *id, uint8_t *payload) {
    // Decode the type.
    *type = frame & FRAME_TYPE_MASK;
    frame >>= FRAME_TYPE_BITS;

    // Decode the identifier.
    *id = frame & FRAME_ID_MASK;
    frame >>= FRAME_ID_BITS;

    // Continue only if there is a payload.
    if (*type == FRAME_TYPE_VALUE_ACKNOWLEDGE)
        return true;

    // Decode the parity.
    uint8_t parity = frame & FRAME_PARITY_MASK;
    frame >>= FRAME_PARITY_BITS;

    // Decode the payload while calculating the parity.
    for (int i = FRAME_PAYLOAD_LENGTH - 1; i >= 0; i--) {
        // Decode the payload byte.
        uint8_t byte = frame & FRAME_PAYLOAD_MASK;
        frame >>= FRAME_PAYLOAD_BITS;
        payload[i] = byte;

        // Update the parity.
        parity ^= byte;
    }

    // Return success only if the parity matches the expected default.
    if (parity != FRAME_PARITY_DEFAULT) {
        debug("[decode] Parity mismatch, expected %u but got %u.\n",
            FRAME_PARITY_DEFAULT, parity);
    }
    return parity == FRAME_PARITY_DEFAULT;
}

void ir_update_inbound() {
    // Do nothing if there is no new frame to process.
    if (!recv_cache_ready)
        return;

    // Grab the frame and unset the ready flag.
    uint32_t frame = recv_cache;
    recv_cache_ready = false;

    // Decode the frame or return if it failed.
    uint8_t type, id;
    uint8_t payload[FRAME_PAYLOAD_LENGTH];

    if (!frame_decode(frame, &type, &id, payload)) {
        debug("[recv] failed to decode frame, ignoring.\n");
        return;
    }

    // Handle acknowledgements.
    if (type == FRAME_TYPE_VALUE_ACKNOWLEDGE) {

        // Ignore acknowledge if we are not waiting for one.
        if (send_is_acknowledged) {
            debug("[recv] Got ACK, but was not expecting one.\n");
            return;
        }

        // Check if the ID is the last ID we sent.
        if (id != send_id) {
            debug("[recv] Got ACK, but for wrong ID: expected %u but got %u\n",
                send_id, id);
            return;
        }

        // Increment the ID for the next packet we will send and set acknowledged
        // to true so we can continue sending.
        send_id = (send_id + 1) % FRAME_ID_MODULO;
        debug("[recv] Got ACK, send_id is now: %u\n", send_id);
        send_is_acknowledged = true;
        return;
    }

    // Check if the identifier has incremented by one.
    if (id != (recv_last_id + 1) % FRAME_ID_MODULO) {

        // If the other party resent this frame but we already acknowledged it,
        // they might not have received our acknowledgement. Send it again.
        if (id == recv_last_id) {
            debug("[recv] got same frame identifier twice; resending ACK.\n");
            send_should_acknowledge = 1;
            return;
        }

        // Otherwise, something must've gone wrong. Ignore the frame.
        debug("[recv] Frame ID incorrect, expected %u but got %u. Ignoring.\n",
            (recv_last_id + 1) % FRAME_ID_MODULO, id);
        return;
    }

    // Once we're certain that the frame is formed correctly, we can
    // accept the payload and insert it into our inbound buffer.
    for (int i = 0; i < FRAME_PAYLOAD_LENGTH; i++) {
        buffer_write(&inbound, payload[i]);
    }

    // Update the last received identifier so the next packet is actually valid.
    recv_last_id = id;
    debug("[recv] recv_last_id is now: %u\n", recv_last_id);

    // Send an acknowledgement as soon as possible.
    send_should_acknowledge = 1;
}

void ir_update_outbound() {
    // Do nothing if we are currently in the process of transmitting a frame.
    if (send_data_bits)
        return;

    // Send an acknowledge as soon as possible if we have to.
    if (send_should_acknowledge) {
        send_should_acknowledge = false;
        debug("[send] Sending ACK\n");

        send_data = frame_encode(FRAME_TYPE_VALUE_ACKNOWLEDGE, recv_last_id, NULL);
        send_data_bits = FRAME_BITS_ACKNOWLEDGE;
        return;
    }

    // Check if we are still waiting for the last frame we sent to be acknowledged.
    if (!send_is_acknowledged) {

        // If too much time has passed, resend our frame.
        if (network_time - send_time > 800) {
            debug("[send] No ACK in time, resending frame.\n");
            debug("[send] data: ");
            debug_bits(send_cache_data, send_cache_data_bits);
            debug("\n");
            send_data = send_cache_data;
            send_data_bits = send_cache_data_bits;
            send_time = network_time;
        }

        // Do nothing else while we're awaiting acknowledgement.
        return;
    }

    // If we're not awaiting acknowledgement, we can perhaps send a new frame.
    // First, check if we have enough data to form a new frame.
    if (buffer_available(&outbound) < FRAME_PAYLOAD_LENGTH)
        return;

    // Read the payload for a new frame.
    uint8_t payload[FRAME_PAYLOAD_LENGTH];
    for (int i = 0; i < FRAME_PAYLOAD_LENGTH; i++) {
        payload[i] = buffer_read(&outbound);
    }

    // Build a new frame. It is also given an ID to use
    debug("[send] send_id: %u\n", send_id);
    uint32_t frame = frame_encode(FRAME_TYPE_VALUE_PAYLOAD, send_id, payload);

    debug("[send] data: ");
    debug_bits(frame, FRAME_BITS_PAYLOAD);
    debug("\n");

    // Load the frame into the transmit 'registers'.
    send_data      = send_cache_data      = frame;
    send_data_bits = send_cache_data_bits = FRAME_BITS_PAYLOAD;
    send_time = network_time;
    send_is_acknowledged = false;
}

bool ir_update() {
    ir_update_inbound();
    ir_update_outbound();

    // Return true unless we've sent data that has gone unacknowledged.
    return send_is_acknowledged;
}

uint16_t ir_read() {
    return (((uint16_t)buffer_read(&inbound)) << 7) | buffer_read(&inbound);
}

void ir_write(uint16_t b) {
    // Only the lowest 7 bits of each byte are preserved during transfer.
    // This means we only have 14 out of the 16 advertised bits!
    buffer_write(&outbound, b >> 7);
    buffer_write(&outbound, b);
}

uint8_t ir_available() {
    return buffer_available(&inbound) / 2;
}

uint8_t ir_space() {
    return buffer_space(&outbound) / 2;
}

ISR(INT0_vect) {
    // Receiving: Note signal change.

    // Read and reset the signal length on any change.
    uint8_t length = recv_signal_length;
    recv_signal_length = 0;

    // Discard our inbound byte if the length of this bit is too long.
    if (length > 10) {
        recv_data = 0;
        recv_data_bits = 0;
    }

    // If the signal became HIGH, we can ignore it.
    if (ir_get_signal())
        return;

    // Store the bit.
    recv_data |= (uint32_t)(length > 4) << recv_data_bits;
    recv_data_bits++;

    // Store the byte, when necessary.
    if (recv_data_bits == FRAME_BITS_PAYLOAD
    || (recv_data_bits == FRAME_BITS_ACKNOWLEDGE
        && (recv_data & FRAME_TYPE_MASK))) {
        debug("[recv] data: ");
        debug_bits(recv_data, recv_data_bits);
        debug("\n");
        recv_cache = recv_data;
        recv_cache_ready = true;
        recv_data = 0;
        recv_data_bits = 0;
    }
}

ISR(TIMER0_COMPA_vect) {
    // Transmitting: Sending the current byte.

    // Maintain the current signal as long as necessary.
    if (send_signal_length) {
        send_signal_length--;
        return;
    }

    // Send a LOW signal between HIGH signals.
    if (send_signal) {
        send_signal = 0;

        // The LOW signal lasts longer between frames.
        send_signal_length = send_data_bits ? 3 : 12;
        ir_transmit_low();
        return;
    }

    // Send the next bit.
    if (send_data_bits) {
        send_signal = 1;
        send_signal_length = (send_data & 1) ? 6 : 2;
        send_data >>= 1;
        ir_transmit_high();
        send_data_bits--;
    }
}

ISR(TIMER0_COMPB_vect) {
    // Receiving: Increment count.
    recv_signal_length++;

    // Increment the network time for determining when to resend data.
    network_time++;
}

void ir_clear() {
    // Discard our inbound bits and any unprocessed inbound byte.
    recv_data_bits = 0;
    recv_signal_length = 0;
    recv_cache_ready = false;

    // Discard our outbound bits.
    send_data_bits = 0;

    // Reset the networking state: don't wait for acknowledgements.
    send_should_acknowledge = false;
    send_is_acknowledged = true;

    // Restart the frame sequence.
    recv_last_id = 0;
    send_id = 1;

    // Reset the time.
    network_time = 0;

    // Discard any pending inbound or outbound data.
    buffer_init(&inbound);
    buffer_init(&outbound);
}

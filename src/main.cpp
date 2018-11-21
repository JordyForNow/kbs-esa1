#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>    // for the touchscreen
#include "Grid.h"





int currentcolor;



int main(void){
  init();
  
  Serial.begin(9600);
  Serial.println("Serial started");
  
  tft.begin();
  Serial.print("TFT started!");
  
  tft.setRotation(1);				// use the screen in landscape
  tft.fillScreen(ILI9341_BLACK);    // paint black background
  
  
  
  grid_init();
  
  
/*
  	cellTypes_t testplayerone = playerOne;
  	cellTypes_t testplayertwo = playerTwo;
  	cellTypes_t testwall = wall;
  	cellTypes_t testbomb = bomb;
  	cellTypes_t testbox = box;
  	
  	_delay_ms(100);
  	drawCell(0,0,testplayerone);
  	_delay_ms(100);
  	drawCell(2,2,testplayertwo);
  	_delay_ms(100);
  	drawCell(4,4,testwall);
  	_delay_ms(100);
  	drawCell(6,6,testbomb);
  	_delay_ms(100);
  	drawCell(15,11,testbox);
  	_delay_ms(100);
  
*/

  
  while(1){
    

	

	
		
		
  }
  
  
  return 0;
}



// AD8232 to 1.8 SPI LCD
// 5/5/2021 by Bob Davis

#include <Wire.h>
#include "MAX30105.h"
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
// TFT LCD pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8
TFT TFTscreen = TFT(cs, dc, rst);
int xpos;
int lanalog;
int manalog;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(6, INPUT); // Setup for leads off detection LO +
  pinMode(7, INPUT); // Setup for leads off detection LO -
  TFTscreen.begin();
  TFTscreen.background(00, 00, 00); 
}
 
void loop() {
  if((digitalRead(6) == 1)||(digitalRead(7) == 1)){
  Serial.println('!');
  }
  else{
  // send the value of analog input 0:
  lanalog=analogRead(A0);
  Serial.println(lanalog);
  }
  xpos=xpos+1;
  if (xpos>160) {
    xpos=0;
  }
  TFTscreen.stroke(0,0,0);   // Black
  TFTscreen.line(xpos,0,xpos,128);  // Clear ahead
  TFTscreen.stroke(00,00,250);  // Red
  TFTscreen.line(xpos-1,(manalog/2)-100,xpos,(lanalog/2)-100); 
  manalog=lanalog;
  //Wait for a bit to keep serial data from saturating
  delay(20);
}

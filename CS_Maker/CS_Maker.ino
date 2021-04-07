/*****************************
Arduino Colloidal Silver Maker
By Bob Davis on April 2020 

Uses a 16x2 LCD display shield or equivalent
Shows the voltage, current, and run time.

The circuit:
 * LCD RS - D9
 * LCD Enable - D8
 * LCD D4 - D4
 * LCD D5 - D5
 * LCD D6 - D6
 * LCD D7 - D7
 * LCD R/W and VSS pin to ground
 * LCD VCC and LED pin to 5V

 * Uses L293 motor controller on D10 and D11 for PWM ability
 * Uses L293 on D3 for the stirrer motor.
 * A1 and A2 are analog feedback from the CS maker
*********************/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the
// interface pins
LiquidCrystal lcd(9, 8, 4, 5, 6, 7);

// Pins for Colloidal silver maker
int CS1=10;
int CS2=11;
// Pins for stirrer
int Stir=3;
int Shutdown=0;
// Variables for time
int hours;
int minutes;
int seconds;
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second = 1000; // 1000 milliseconds in a second
float AN1=0.0; // Analog inut 1
float AN2=0.0;
float temp1=0.0;
float temp2=0.0;
float CUR=0.0;   // Current in ma
float MCUR=0.0;  // Memory Current in ma
int CurSet=255;  // Current Setting

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode (CS1, OUTPUT);
  pinMode (CS2, OUTPUT);
  pinMode (Stir, OUTPUT);
}

void loop() {
  // Reverse current every 30 minutes
  if (Shutdown==0){
    analogWrite(Stir, 128); // 1/2 supply voltage
    if (minutes<30){   // Reverse every 30 minute
      analogWrite(CS1, 0);
      analogWrite(CS2, CurSet);
    }
    else{
      analogWrite(CS2, 0);
      analogWrite(CS1, CurSet);
    }
  }
  else{  // Shutdown
    analogWrite(CS1, 0);
    analogWrite(CS2, 0); 
    analogWrite(Stir, 0); 
    }
  temp1=analogRead(A1);
  temp2=analogRead(A2);
  AN1=((temp1*5.0)/1024.0)*4.0; // Convert to volts
  AN2=((temp2*5.0)/1024.0)*4.0;
  CUR=abs(AN1-AN2);
  if (((CUR+MCUR)/2) > 1.0) {  // Reduce PWM, current
    CurSet--;
    } 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("V:");
  lcd.print(AN1);
  lcd.setCursor(8,0);
  lcd.print("V:");
  lcd.print(AN2);
  lcd.setCursor(10,1);
  lcd.print("C:");
  lcd.print(CUR);
  // print the number of seconds since reset:
  long timeNow = millis();
  hours = (timeNow) / hour;             
  minutes = ((timeNow) % hour) / minute ;
  seconds = (((timeNow) % hour) % minute) / second;
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds);

  if (hours>3){ // Time is 4 hours
    Shutdown=1;
    }
  if (((CUR+MCUR)/2)>2.0){ // Average Current over 2ma
    Shutdown=1;
    }
  MCUR=CUR;   // Save currnet for averaging
  delay(300);
}

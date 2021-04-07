/*
  Optical Heart Rate Detection using the MAX30105 Breakout
  Based on: Nathan Seidle @ SparkFun Electronics
  BY Bob Davis on  3/29/2021
   This is a demo to show the reading of heart rate or beats per minute (BPM) 
  on a Nokia PCD8544 LCD Screen
  
  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected
*/

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "MAX30102_PulseOximeter.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 3, 4);
MAX30105 particleSensor;

int ypos=0;     // Trace Y axis
int StartSample = 0;
int EndSample = 0;
int rate[5]; // Array of samples in Milliseconds (MS)
int MS = 0;  // Milliseconds between pulses
int BPM;     // Beats Per Minute
long peak=80000;   // initial Peak 
long valley=80000; // Minimum valley
long Mvalley=80000; // Memory valley
long redValue=80000;
long irValue=80000;
long MredValue=80000;
long MirValue=80000;
boolean Pulse = false; // "True" when heartbeat detected
boolean Beat = false;
int rateTotal = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");
  display.begin();
  display.setContrast(50);
  display.clearDisplay();   // clears the screen and buffer

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure
 
  Serial.print("IR");
  Serial.print('\t');
  Serial.println("RED");
}

void loop() {
  irValue = particleSensor.getIR();
  redValue = particleSensor.getRed();
  // Find peak, valley and detect change in direction 
  if (redValue > peak) peak=redValue;  // Find peak
  if (redValue < valley) valley=redValue; // Find valley
  if ((redValue > MredValue) && (Pulse == false)){ // Pulse Detected
    peak = valley+((peak-valley)/2);
    Pulse = true; // set Pulse flag
    Beat = true;
  }

  if ((redValue < MredValue) && (Pulse == true)){ // Pulse Finished
    Pulse = false; // reset Pulse flag
    EndSample = millis();
    MS = (EndSample-StartSample);
    StartSample = millis();
    // Reset valley to center
    valley = valley+((peak-valley)/2);
    // Keep and average a running total
    rate[5] = rate[4]; // Shift the oldest MS values
    rate[4] = rate[3]; // Shift the oldest MS values
    rate[3] = rate[2]; // Shift the oldest MS values
    rate[2] = rate[1]; // Shift the oldest MS values
    rate[1] = MS; // add the latest MS to array
    rateTotal = (rate[1]+rate[2]+rate[3]+rate[4]+rate[5])/5; // Add up the MS values
    BPM = 60000/rateTotal; // Beats in a minute is BPM
  }

  Serial.print(constrain(irValue, 75000, 100000)); //Send raw data to plotter
  Serial.print('\t');
  Serial.println(constrain(redValue, 75000, 100000)); //Send raw data to plotter

  // Draw the trace of heartbeat
  display.drawLine(ypos-1,((MredValue-Mvalley)/25),ypos,((redValue-valley)/25),BLACK);  
  ypos=ypos+1;
  if (ypos>84) {
    ypos=0;
    display.clearDisplay();   // clears the screen and buffer
  }
  if (Beat=true){
    display.fillRect(0,40,44,8,WHITE); //Clear box
    display.setCursor(0,40);  // Bottom line
    display.println("BPM: ");
    display.setCursor(24,40);  // Bottom line
    display.println(BPM);
    Beat=false;
  }
  display.display();  // Update the screen
  MredValue=redValue;
  MirValue=irValue;
  Mvalley=valley;
}

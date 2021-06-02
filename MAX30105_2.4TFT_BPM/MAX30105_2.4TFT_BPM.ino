/*
  Optical Heart Rate Detection using the MAX30105 Breakout
  Based on: Nathan Seidle @ SparkFun Electronics
  BY Bob Davis on 3/29/2021
  This is a demo to show the reading of heart rate or beats per minute (BPM) 
  on a 2.4" LCD Screen
  
  Hardware Connections (MAX30102 to Arduino):
  5V = 5V 
  GND = GND
  SDA = A4 (or SDA)
  SCL = A5 (or SCL)
  INT = Not connected

 Use these 8 data lines to connect to the LCD:
 For the Arduino Uno, Duemilanove, Diecimila, etc.:
   D0 connects to digital pin 8  (Notice these are
   D1 connects to digital pin 9   NOT in order!)
   D2 connects to digital pin 2
   D3 connects to digital pin 3
   D4 connects to digital pin 4
   D5 connects to digital pin 5
   D6 connects to digital pin 6
   D7 connects to digital pin 7

 The control pins for the LCD can be assigned to any digital or
 analog pins...but we'll use the analog pins. 
*/
#include <Wire.h>
#include "MAX30105.h"
#include <SPI.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
// TFT LCD pin definition for the Uno
#define LCD_RESET A7 // FAKE-Connect LCD Reset to Arduino's reset 
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

MAX30105 particleSensor;
// Variables for pulse rate
int StartSample;
int EndSample;
int rate[5]; // Array of samples in Milliseconds (MS)
int rateTotal;
int MS;  // Milliseconds between pulses
int BPM;     // Beats Per Minute
// Peak and valley detection
long peak=80000;       // initial Peak 
long valley=80000;     // Minimum valley
long irpeak=80000;     // initial Peak 
long irvalley=80000;   // Minimum valley
long redValue;   // Raw Red
long irValue;    // Raw IR
// variables for SPO2
float RedACValue;     // RED AC Component
float IRACValue;      // IR AC Component
float RedACDC;
float IRACDC;
int SPO2R;
int spo2av[5];
int SPO2A;
long SumRed;
long SumIR;
int SampleNo;
// Memory values
long Mvalley=80000;    // Memory valley
long Mirvalley=80000;  // Memory IR valley
long MredValue=80000;  // Memory red value
long MredValue2=80000; // 2 deep memory
long MirValue=80000;   // Memory IR value
boolean Pulse = false; // "True" when heartbeat detected
// Screen variables
boolean SBeat = false; // Beat detected update screen
long Sred;    // Average red
long MSred;   // Memory Average red
long Sir;     // Average ir
long MSir;    // Memory Average ir
int xpos=0;   // Trace X axis

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");
  // initialize the display, clear screen
  uint16_t identifier = 0x9341;
  // Options: 0x8357 0x7575 0x7575 0x9328 0x9325
//  tft.reset();
  tft.begin(identifier); 
  tft.fillScreen(BLACK);
  tft.setRotation(1);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to FF=50mA
  byte sampleAverage = 8; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure
  // Set up serial plotter
  Serial.print("IR");
  Serial.print('\t');
  Serial.println("RED");
}

void loop() {
  irValue = particleSensor.getIR();
  redValue = particleSensor.getRed();
  // Find peak, valley and detect change in direction 
  if (redValue > peak) peak=redValue;     // Find peak
  if (irValue > irpeak) irpeak=irValue;     // Find peak
  if (redValue < valley) valley=redValue; // Find valley
  if (irValue < irvalley) irvalley=irValue; // Find ir valley
  SumRed=SumRed+redValue-valley;
  SumIR=SumIR+irValue-irvalley;
  SampleNo++;
  if ((redValue > MredValue2) && (Pulse == false)){ // Pulse Detected
    Sred = (peak+valley)/2; // average of peak and valley
    Sir = (irpeak+irvalley)/2; // average of peak and valley
    
    // Calculate SPO2
    RedACValue = peak-valley;
    IRACValue = irpeak-irvalley;
    RedACDC = RedACValue/(SumRed/SampleNo); //Sum/No=average or DC
    IRACDC = IRACValue/(SumIR/SampleNo);
    float Z = (RedACDC/IRACDC); // Ratio
    SPO2R = (-30*Z + 30)*Z + 95; // Modified formula 

    // Keep and average a running total
    spo2av[5] = spo2av[4]; // Shift the oldest reading
    spo2av[4] = spo2av[3]; // Shift the oldest reading
    spo2av[3] = spo2av[2]; // Shift the oldest reading
    spo2av[2] = spo2av[1]; // Shift the oldest reading
    spo2av[1] = SPO2R; // add the latest reading to array
    SPO2A = ((spo2av[1]+spo2av[2]+spo2av[3]+spo2av[4]+spo2av[5])/5); // Average
    SumRed=0;
    SumIR=0;
    SampleNo=0;
 
    peak = (peak+valley)/2; // Reset peak
    irpeak = (irpeak+irvalley)/2; // Reset ir peak
    Pulse = true; // set Pulse flag
    SBeat = true; // Update screen flag
  }

  if ((redValue < MredValue2) && (Pulse == true)){ // Pulse Finished
    Pulse = false; // reset Pulse flag
    EndSample = millis();
    MS = (EndSample-StartSample);
    StartSample = millis();
        
    valley = (peak+valley)/2; // Reset valley 
    irvalley = (irpeak+irvalley)/2; // Reset IR valley
    // Keep and average a running total
    rate[5] = rate[4]; // Shift the oldest MS values
    rate[4] = rate[3]; // Shift the oldest MS values
    rate[3] = rate[2]; // Shift the oldest MS values
    rate[2] = rate[1]; // Shift the oldest MS values
    rate[1] = MS; // add the latest MS to array
    rateTotal = ((rate[1]+rate[2]+rate[3]+rate[4]+rate[5])/5); // Add up the MS values
    BPM = 60000/rateTotal; // Beats in a minute is BPM
  }

  Serial.print(constrain(irValue, 80000, 120000)); //Send raw data to plotter
  Serial.print('\t');
  Serial.println(constrain(redValue, 80000, 120000)); //Send raw data to plotter

  // Clear Ahead then Draw the trace of heartbeat
  tft.drawRect(xpos,0,1,240,BLACK);  // Clear ahead
  tft.drawLine(xpos-1,(((MredValue-MSred)/5)+80),xpos,(((redValue-Sred)/5)+80),RED); 
  tft.drawLine(xpos-1,(((MirValue-MSir)/5)+160),xpos,(((irValue-Sir)/5)+160),BLUE);  
 
  xpos=xpos+1;
  if (xpos>239) {
    xpos=0;
  }
  if (SBeat==true){

//    TFTscreen.fill(0,0,0);
    tft.setTextSize(2);
    tft.fillRect(240,0,80,240,BLACK);  // Clear right side
    tft.setTextColor(WHITE);
    tft.drawRect(245,15,65,45,GREEN);  // Clear right side
    tft.drawRect(245,65,65,45,GREEN);  // Clear right side
    tft.drawRect(245,115,65,45,GREEN);  // Clear right side
    tft.drawRect(245,165,65,45,GREEN);  // Clear right side
    tft.setCursor(250,20);  // Right Side
    tft.println("BPM:");
    tft.setCursor(250,38); 
    tft.println(BPM);
    tft.setCursor(250,70);  
    tft.println("Red:");
    tft.setCursor(250,88); 
    tft.println(RedACValue,0);    
    tft.setCursor(250,120); 
    tft.println("IR:");
    tft.setCursor(250,138);    
    tft.println(IRACValue,0);
    tft.setCursor(250,170);
    tft.println("SPO2:");
    tft.setCursor(250,188);
    tft.println(SPO2A);
    
    SBeat=false;
  }
  MredValue2=MredValue;
  MredValue=redValue;
  MirValue=irValue;
  Mvalley=valley;
  Mirvalley=irvalley; 
  MSred=Sred; 
  MSir=Sir; 
}

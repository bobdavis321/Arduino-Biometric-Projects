// Heartbeat display 
// Hearteat BPM displays on line 1
// bar Graph of Heartbeat displays on line 2
// By Bob Davis in April 2020

#include <LiquidCrystal.h>
// Variables
int pulsePin = A0; // Pulse Sensor on analog pin 0
int blinkPin = D13; // pin to blink led at each beat
int StartSample = 0;
int EndSample = 0;
int rate[5]; // Array of samples in Milliseconds (MS)
int MS = 0;  // Milliseconds between pulses
int BPM;     // Beats Per Minute
int peak=260;   // Peak voltage
int valley=240; // Minimum voltage
int thresh=250; // Trigger threashold
int sens=80;  // Sensitivity to rise and fall of heartbeat
int Signal;  // Incoming raw data from heart sensor
boolean Pulse = false; // "True" when heartbeat detected
int rateTotal = 0;

// LiquidCrystal lcd(13,12,6,5,4,3);//
LiquidCrystal lcd(D8, D9, D4, D5, D6, D7);// LCD shield pinout

// Create graphics lines
byte line1[8]= {B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000};
byte line2[8]= {B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000};
byte line3[8]= {B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100};
byte line4[8]= {B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110};
byte line5[8]= {B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111};

void setup(){
 pinMode(blinkPin,OUTPUT); // pin to blink with heartbeat
 pinMode(pulsePin,INPUT); // Configuring pin A0 as input
 lcd.begin(16,2);  // 16 by 2 LCD
 Serial.begin(9600);
 // Send the custom characters to the LCD.
 lcd.createChar(1, line1);
 lcd.createChar(2, line2);
 lcd.createChar(3, line3);
 lcd.createChar(4, line4);
 lcd.createChar(5, line5);
}

void loop(){
 Signal = analogRead(pulsePin);
 // Display values of BPM Signal on LCD
 lcd.clear();
 lcd.setCursor(0,0);  // First line
 lcd.print("BPM:");
 lcd.print(BPM);
 lcd.setCursor(8,0);  // First line
 lcd.print("MS:");
 lcd.print(MS); 
 // Draw Bar Graph of heartbeat
 lcd.setCursor(0,1);  // Second line LCD=Bargraph
 // Draw lower spaces as solid for the bargraph
 int SSignal =Signal/8; // Scale value
 for (int pos=0; pos < SSignal/10; pos++) {lcd.write(5);}
 lcd.setCursor(SSignal/10, 1);
 if (SSignal%10/2 == 0) lcd.write(1);
 if (SSignal%10/2 == 1) lcd.write(2);
 if (SSignal%10/2 == 2) lcd.write(3);
 if (SSignal%10/2 == 3) lcd.write(4);
 if (SSignal%10/2 == 4) lcd.write(5);

 // Find peak, valley and detect change in direction 
 if (Signal > peak) peak=Signal;  // Find peak
 if (Signal < valley) valley=Signal; // Find valley
 if (Pulse == false) thresh = (valley+sens); // look for rise
 if (Pulse == true) thresh = (peak-sens);    // look for fall

 if ((Signal > thresh) && (Pulse == false)){ // Pulse Detected
  Pulse = true; // set Pulse flag
  digitalWrite(blinkPin,HIGH); // turn on pin 13 LED
 }

 if ((Signal < thresh) && (Pulse == true)){ // Pulse Finished
  Pulse = false; // reset Pulse flag
  digitalWrite(blinkPin,LOW); // turn off pin 13 LED
  EndSample = millis();
  MS = (EndSample-StartSample);
  StartSample = millis();
  // Reset peak and valley to center
  valley = valley+((peak-valley)/2);
  peak = valley+((peak-valley)/2);
  // BPM = 60000/MS;
  // Keep and average a running total
  rate[5] = rate[4]; // Shift the oldest MS values
  rate[4] = rate[3]; // Shift the oldest MS values
  rate[3] = rate[2]; // Shift the oldest MS values
  rate[2] = rate[1]; // Shift the oldest MS values
  rate[1] = MS; // add the latest MS to array
  rateTotal = (rate[1]+rate[2]+rate[3]+rate[4]+rate[5])/5; // Add up the MS values
  BPM = 60000/rateTotal; // Beats in a minute is BPM

  // display results on computer screen for troubleshooting.
  Serial.print("Beats Per Min=");
  Serial.print("\t");
  Serial.print(BPM);
  Serial.print("\t");
  Serial.print(rate[1]);
  Serial.print("\t");
  Serial.print(rate[2]);
  Serial.print("\t");
  Serial.print(rate[3]);
  Serial.print("\t");
  Serial.print(rate[4]);
  Serial.print("\t");
  Serial.print(rate[5]);
  Serial.println();  
 }  
 delay(100); // take a break
}

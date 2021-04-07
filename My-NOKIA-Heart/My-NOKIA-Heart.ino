// NOKIA Heartbeat 
// Hearteat BPM displays on line 1
// Scope Trace of Heartbeat displays on lower 1/2
// By Bob Davis in April 2020

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
Adafruit_PCD8544 display = Adafruit_PCD8544(D7, D6, D5, D3, D4);

// Variables
int pulsePin = A0; // Pulse Sensor on analog pin 0
int blinkPin = D13; // pin to blink led at each beat
int StartSample = 0;// Start time MS
int EndSample = 0;  // End time MS
int rate[5];    // Array of samples in Milliseconds (MS)
int MS = 0;     // Milliseconds between pulses
int BPM;        // Beats Per Minute
int peak=800;   // Typical Peak voltage
int valley=500; // Typical Minimum voltage
int thresh=250; // Trigger threashold
int sens=70;   // Sensitivity to rise and fall of heartbeat
int Signal;     // Incoming raw data from heart sensor
int ypos=0;     // Trace Y axis
boolean Pulse = false; // "True" when heartbeat detected
int rateTotal = 0;

void setup(){
  Serial.begin(9600);
  display.begin();
  display.setContrast(50);
  display.clearDisplay();   // clears the screen and buffer 
  pinMode(blinkPin,OUTPUT); // pin to blink with heartbeat
  pinMode(pulsePin,INPUT); // Configuring pin A0 as input
}

void loop(){
  Signal = analogRead(pulsePin);
  // Display values of BPM Signal on LCD
  display.fillRect(0,0,80,20,WHITE); //Clear top
  display.setCursor(0,0);  // First line
  display.println("BPM:");
  display.setCursor(24,0);  // First line
  display.println(BPM);
  display.setCursor(0,10);  // First line
  display.println("MS:");
  display.setCursor(24,10);  // First line
  display.println(MS);
  // Draw the trace of heartbeat
  display.drawPixel(ypos,(Signal/10)-40,BLACK);  // Bottom of LCD
  ypos=ypos+1;
  if (ypos>84) {
    ypos=0;
    display.clearDisplay();   // clears the screen and buffer 
  }
  display.display();  // Update the screen

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
    Serial.println();  
  }  
  delay(10); // take a break
}

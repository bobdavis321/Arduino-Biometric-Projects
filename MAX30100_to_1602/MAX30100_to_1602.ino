/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>
Added LCD by Bob Davis 
 */
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation 
PulseOximeter pox;
uint32_t tsLastReport = 0;
// Callback (registered below) fired when a pulse is detected 
void onBeatDetected()
{
    Serial.println("Beat!");
    lcd.setCursor(12,0);
    lcd.print("Beat");
}
 
void setup()
{
    Serial.begin(9600);
    Serial.print("Initializing..");
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.print("Initializing...");
    delay(3000);
    lcd.clear();
    // Initialize the PulseOximeter instance
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    // The default current for the IR LED is 50mA and is changed below
    pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
    // Register a callback for the beat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
    
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Rate:");
        lcd.print(pox.getHeartRate());
        lcd.setCursor(0,1);
        lcd.print("SpO2:");
        lcd.print(pox.getSpO2());
        lcd.print("%");
         
        tsLastReport = millis();
    }
}

/**************************************************************************
SSD1306 displaying two DHT11's

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
// Modified for OLED and dual sensors by Bob davis on 4-26-2021
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN 2 and 3
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY 

 **************************************************************************/
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 3    // Digital pin connected to the DHT2 sensor

// Uncomment whatever type of sensor you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensors.
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D=128x64, 0x3C=128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  display.begin();
  display.fillScreen(BLACK);
  dht.begin();
  dht2.begin();
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float hum = dht.readHumidity();
  float hum2 = dht2.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float tem = dht.readTemperature(true);
  float tem2 = dht2.readTemperature(true);

  // Check if read failed and try again.
  if (isnan(hum) || isnan(tem)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("   Indoor ");
  display.setCursor(0,10);
  display.print("Humidity: ");
  display.print(hum);
  display.print(" % ");
  display.setCursor(0,20);
  display.print("Degrees: ");
  display.print(tem);
  display.print(" F ");

  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,30);
  display.print("   Outdoor ");
  display.setCursor(0,40);
  display.print("Humidity: ");
  display.print(hum2);
  display.print(" % ");
  display.setCursor(0,50);
  display.print("Degrees: ");
  display.print(tem2);
  display.print(" F ");

  display.display();
  // For serial monitor:
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" ");
  Serial.print(hum2);
  Serial.print("%  Temperature: ");
  Serial.print(tem);
  Serial.print(" ");
  Serial.print(tem2);
  Serial.print("°F");
  Serial.print('\n');  //new line
  
}

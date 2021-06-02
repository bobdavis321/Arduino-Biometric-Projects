// 2 DHT11 to OLED

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
// Modified for OLED and dual sensors by Bob davis on 4-26-2021
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN 2 and 3
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)

#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

// You can use any (4 or) 5 pins
// This pin arrangement makes possible the use of a header extender
#define sclk D5
#define mosi D4
#define rst  D3
#define dc   D2
#define cs   D1

// Color definitions
#define BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTPIN2 3    // Digital pin connected to the DHT2 sensor

// Uncomment whatever type of sensor you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Initialize DHT sensors, OLED.
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
//Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, cs, dc, rst);
Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  display.begin();
  display.fillScreen(BLACK);
  dht.begin();
  dht2.begin();
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
//    return;
  }
  display.fillScreen(BLACK);
  display.setTextColor(RED);
  display.setCursor(0,0);
  display.print("   Indoor ");
  display.setCursor(0,10);
  display.print("Humidity: ");
  display.print(hum);
  display.setCursor(0,20);
  display.print("Degrees: ");
  display.print(tem);

  display.setTextColor(GREEN);
  display.setCursor(0,30);
  display.print("   Outdoor ");
  display.setCursor(0,40);
  display.print("Humidity: ");
  display.print(hum2);
  display.setCursor(0,50);
  display.print("Degrees: ");
  display.print(tem2);

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

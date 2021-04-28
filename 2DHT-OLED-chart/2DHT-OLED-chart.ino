// 2 DHT11 to OLED with chart
// Uses 96x64 SSD1331 OLED
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
// Modified for OLED and dual sensors by Bob davis on 4-26-2021
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to your DHTPIN 2 and 3
// Connect pin 3 of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 of the sensor to GROUND (if your sensor has 4 pins)

#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

// You can use any (4 or) 5 pins
// This pin arrangement makes possible the use of a header extender
#define sclk 13
#define mosi 11
#define rst  10
#define dc   9
#define cs   8

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
Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, cs, dc, rst);

float temold;
float tem2old;
int x = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  display.begin();
  display.fillScreen(BLACK);
  dht.begin();
  dht2.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' 
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
  display.fillRect(0, 0, 96, 20, BLACK);
  display.setTextColor(RED);
  display.setCursor(0,0);
  display.print("I ");
  display.print(hum);
  display.print("% ");
  display.print(tem);
  display.print("F");

  display.setTextColor(GREEN);
  display.setCursor(0,10);
  display.print("O ");
  display.print(hum2);
  display.print("% ");
  display.print(tem2);
  display.print("F");
 
  // Graph temperature change
  x++;
  if (x > 96) { x=0; }
  display.drawLine(x, 20, x, 64, BLACK);
  display.drawLine(x+1, 20, x+1, 64, BLACK);
  int y=map(tem,30,99,64,20);
  int y1=map(temold,30,99,64,20);
  display.drawLine(x, y, x+1, y1, RED);
  y=map(tem2,30,99,64,20);
  y1=map(tem2old,30,99,64,20);
  display.drawLine(x, y, x+1, y1, GREEN);
  
  // Save temperatures
  temold=tem;
  tem2old=tem2;
  
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
  delay(2000);
}

// BME-280 to OLED
// 6-22-2021 by Bob Davis
// Uses Three Adafruit Libraries

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <Wire.h>

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

// Initialize Sensors, OLED.
Adafruit_SSD1331 display = Adafruit_SSD1331(&SPI, cs, dc, rst);
Adafruit_BME280 bme; // use I2C interface
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();

void setup() {
  Serial.begin(9600);
  Serial.println(F("BME280 test!"));
  display.begin();
  display.fillScreen(BLACK);
  if (!bme.begin()) {
   Serial.println(F("Could not find a valid BME280 sensor"));
  }
  bme_temp->printSensorDetails();
  bme_pressure->printSensorDetails();
  bme_humidity->printSensorDetails();
}

void loop() {
  sensors_event_t temp_event, pressure_event, humidity_event;
  bme_temp->getEvent(&temp_event);
  bme_pressure->getEvent(&pressure_event);
  bme_humidity->getEvent(&humidity_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");
  Serial.print(F("Humidity = "));
  Serial.print(humidity_event.relative_humidity);
  Serial.println(" %");
  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");
  Serial.println();

  display.fillScreen(BLACK);
  display.setTextColor(RED);
  display.setCursor(0,0);
  display.print("Temp. C: ");
  display.print(temp_event.temperature);
  display.setTextColor(BLUE);
  display.setCursor(0,15);
  display.print("Temp. F: ");
  display.print((temp_event.temperature*9/5)+32);
  display.setTextColor(GREEN);
  display.setCursor(0,30);
  display.print("Pressure: ");
  display.print(pressure_event.pressure);
  display.setTextColor(YELLOW);
  display.setCursor(0,45);
  display.print("Humidity: ");
  display.print(humidity_event.relative_humidity);
  delay(2000);
}

/**************************************************************************
D1 SSD1306 displaying two DHT11's and sending results to webpage

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
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Set WiFi credentials - use your credentials
#define WIFI_SSID "BobDavis"
#define WIFI_PASS "Teddyteddy"

#define DHTPIN D5     // Digital pin connected to the DHT sensor
#define DHTPIN2 D6    // Digital pin connected to the DHT2 sensor

// Uncomment whatever type of sensor you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

float tem1;
float tem2;
float hum1;
float hum2;

  
// Initialize DHT sensors
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
// Initialize webserver
ESP8266WebServer webserver(80);

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL)
// On an arduino D1:        D2(SDA),  D1(SCL)
#define OLED_RESET    0 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D=128x64, 0x3C=128x32
Adafruit_SSD1306 display(OLED_RESET); // Works with D1 for some reason
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  display.begin();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  dht.begin();
  dht2.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(100); }
 
  // WiFi Connected
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start Web Server
  webserver.on("/", rootPage);
  webserver.begin();
}

void loop() {
  webserver.handleClient(); 
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  hum1 = dht.readHumidity();
  hum2 = dht2.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  tem1 = dht.readTemperature(true);
  tem2 = dht2.readTemperature(true);

  display.clearDisplay();
  display.setCursor(0,0);
  display.print("I Humidity: ");
  display.print(hum1);
  display.print(" % ");
  display.setCursor(0,8);
  display.print("I Degrees: ");
  display.print(tem1);
  display.print(" F ");

  display.setCursor(0,16);
  display.print("O Humidity: ");
  display.print(hum2);
  display.print(" % ");
  display.setCursor(0,24);
  display.print("O Degrees: ");
  display.print(tem2);
  display.print(" F ");

  display.display();
  // For serial monitor:
  Serial.print("Humidity: ");
  Serial.print(hum1);
  Serial.print(" ");
  Serial.print(hum2);
  Serial.print("%  Temperature: ");
  Serial.print(tem1);
  Serial.print(" ");
  Serial.print(tem2);
  Serial.print("°F");
  Serial.print('\n');  //new line
}

void rootPage() { 
  String strTemp, strHum, strTemp2, strHum2;
  char tmp[10];

  dtostrf(tem1,1,2,tmp);
  strTemp = tmp;
  dtostrf(hum1,1,2,tmp);
  strHum = tmp;
  dtostrf(tem2,1,2,tmp);
  strTemp2 = tmp;
  dtostrf(hum2,1,2,tmp);
  strHum2 = tmp;
  
  webserver.send(200,"text/plain","ESP8266 D1 Indoor+Outdoor\n\rIndoor\n\rTemperature "+strTemp+"\n\rHumidity "+strHum+ "\n\rOutdoor\n\rTemperature "+strTemp2+"\n\rHumidity "+strHum2); 
}

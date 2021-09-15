#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 0  // GPIO0
//Adafruit_SSD1306 OLED(OLED_RESET);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 
 
void setup()   {
  display.begin();
  display.clearDisplay();

  //Add stuff into the 'display buffer'
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("TESTING              THIS IS A TEST       1234567890           LINE 4");
 
  display.display(); //output 'display buffer' to screen  
} 
 
void loop() {
}

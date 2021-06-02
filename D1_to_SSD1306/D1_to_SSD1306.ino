#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);
 
void setup()   {
  OLED.begin();
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
//  OLED.setTextWrap(false);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println("TESTING              THIS IS A TEST            1234567890");
 
  OLED.display(); //output 'display buffer' to screen  
//  OLED.startscrollleft(0x00, 0x0F); //make display scroll 
} 
 
void loop() {
}

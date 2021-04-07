/*************************************************** 
  This is for the MLX90614 Temp Sensor
  it displays the results on a 1602 LCD
  These sensors use I2C to communicate
  
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  1602 support added by Bob Davis
 ****************************************************/
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  Serial.println("Adafruit MLX90614 test");  
  mlx.begin();  
}

void loop() {
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
  Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("*F");
  Serial.println();

  lcd.setCursor(0, 0);
  lcd.print("Ambient: ");
  lcd.print(mlx.readAmbientTempF()); 
  lcd.setCursor(0, 1);
  lcd.print("Object: ");
  lcd.print(mlx.readObjectTempF()); 
  delay(500);
}


//i2C display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  lcd.begin();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(1, 0);
  lcd.print("PPM Status");
  lcd.setCursor(1, 1);
  lcd.print("7   VERY LOW");
  delay(3000);
  lcd.clear();
  
  lcd.setCursor(1, 0);
  lcd.print("PPM Status");
  lcd.setCursor(1, 1);
  lcd.print("38   MID");
  delay(3000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("PPM Status");
  lcd.setCursor(1, 1);
  lcd.print("155 HIGH");
  delay(3000);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("PPM Status");
  lcd.setCursor(1, 1);
  lcd.print("202  VERY HIGH");
  delay(3000);
  lcd.clear();
}

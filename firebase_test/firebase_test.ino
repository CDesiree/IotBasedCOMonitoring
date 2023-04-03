
//WIFI settings to Firebase Server
#include <ESP8266WiFi.h>;
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "comonitoring-526d0-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "DYawzVjYhMHgmVHgPZixYTq0LviBQ3sa2UFUsXK0"

const char* ssid = "ZTE_2.4G_9jrwUL";
const char* password = "Gp666Mfv";

// Calibrate
const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int finalRead = 0;
int filterValue = 0;
int count=1;

const unsigned char beep_pin = 2;
int gap=1000;

int inputPin = A0;

//i2C display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

//

void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // initialize LCD
  lcd.begin();
  lcd.backlight();
  
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  //start
  startUp();
  lcd.clear();
  //try
  //filter();
  //sound
  pinMode(beep_pin, OUTPUT);
}

void loop() {
  lcd.setCursor(0, 0); //position of characters to be displayed
  lcd.print("PPM: ");
  lcd.print(finalRead);
  //filter(); //function that filters the data to respective levels
  Calibrate(); //smoothing of raw data
  filter(); //filtering of data according to PPM levels
  IoT(); //uploading sensor data to firebase
  delay(1000);
}

void IoT(){
  Firebase.setInt ("/FirebaseCO/CO_Value",finalRead);
  Firebase.setInt ("/FirebaseCO/Filter_Value", filterValue);
  delay(1000);
}

void Calibrate(){
    // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits

  finalRead=average-analogRead(inputPin);

  if(finalRead<=0){
    finalRead=0;
  }

  //Serial.print("Actual: ");
  //Serial.println(analogRead(A0));
  //Serial.print("Average: ");
  //Serial.println(average);
  Serial.print("Final: ");
  Serial.println(finalRead);
  delay(1000);        // delay in between reads for stability
}

void startUp(){
  int sec=0;
  for(sec=0; sec<=10; sec++){
    lcd.clear();
    lcd.print("Initializing");
    lcd.setCursor(0, 1);
    lcd.print(10-sec);
    lcd.setCursor(3, 1);
    lcd.print("sec");
    delay(1000);
  }
}

//void ppm(){
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("PPM: ");
  //lcd.print(finalRead);
  //filter();
  //delay(1000);
//}

void filter(){
  if(finalRead<=20){
    if(count<30){
      low();
      lcd.setCursor(14, 1);
      lcd.print(count);
      ++count;
    }
    else
    veryHigh();
  }
  else if(finalRead>20 && finalRead<=80){
    if(count<10){
      mid();
      lcd.setCursor(14, 1);
      lcd.print(count);
      ++count;
    }
    else
    veryHigh();
  }
  else if(finalRead>80 && finalRead<=200){
    if(count<5){
      high();
      lcd.setCursor(14, 1);
      lcd.print(count);
      ++count;
    }
    else
    veryHigh();
  }
  else if(finalRead>200 && finalRead<=500){
    veryHigh();
  }
  delay(1000);
}

//filtering
void low(){
  lcd.setCursor(0, 1);
  lcd.print("CO level: LOW");
  filterValue == 1;
}

void mid(){
  lcd.setCursor(0, 1);
  lcd.print("CO level: MID");
  filterValue == 2;
}

void high(){
  lcd.setCursor(0, 1);
  lcd.print("CO level: HIGH");
  filterValue == 3;
}

void veryHigh(){
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("DANGER");
    lcd.setCursor(11, 1);
    filterValue == 4;
    sound();
    ESP.restart();
}

//sound
void sound(){
  for (int count=1;count<=10;count++)
  {
    risefall();
  }
  noTone(beep_pin);//
  delay(gap);
  for (int count=1;count<=10;count++)
  {
    fall(300);
  } 
  noTone(beep_pin);//
  delay(gap); 
  for (int count=1;count<=5;count++)
  {
    fall(600);
  }
  noTone(beep_pin);//
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    rise();
  }
  noTone(beep_pin);//
  delay(gap); 
  for (int count=1;count<5;count++)
  {
    twotone();
  }
  noTone(beep_pin);//
  delay(gap); 
  for (int count=1;count<10;count++)
  {
  zap1();//
  }
  noTone(beep_pin);
  delay(gap); 
  for (int count=1;count<10;count++)
  {
    zap2();//
  }
  noTone(beep_pin);
  delay(gap);
}

//tones
void zap1()
{
    for (float f=3000;f>40;f=f*0.93){
    tone(beep_pin,f);
    delay(10);
  }
}

void zap2()
{
    for (float f=3000;f>10;f=f*0.85){
    tone(beep_pin,2*f);
    delay(5);
    tone(beep_pin,f);
    delay(5); 
  }
}
void risefall()
{
  float rise_fall_time=180;
  int steps=50;
  float f_max=2600;
  float f_min=1000;
  float delay_time=rise_fall_time/steps;
  float step_size=(f_max-f_min)/steps;
  for (float f =f_min;f<f_max;f+=step_size){
    tone(beep_pin,f);
    delay(delay_time);
  }
   for (float f =f_max;f>f_min;f-=step_size){
    tone(beep_pin,f);
    delay(delay_time);
  }
}
void fall(float rise_fall_time)
{
  int steps=50;
  float f_max=2000;
  float f_min=500;
  float delay_time=rise_fall_time/steps;
  float step_size=0.97;
  for (float f =f_max;f>f_min;f*=step_size){
    tone(beep_pin,f);
    delay(delay_time);
  }
}
void rise()
{
  float rise_fall_time=2000;
  int steps=100;
  float f_max=1500;
  float f_min=500;
  float delay_time=rise_fall_time/steps;
  float step_size=1.012;
  for (float f =f_min;f<f_max;f*=step_size){
    tone(beep_pin,f);
    delay(delay_time);
  }
  noTone(beep_pin);
  delay(100);
  
}

void twotone()
{
  float f_max=1500;
  float f_min=1000;
  float delay_time=800;
  tone(beep_pin,f_max);
  delay(delay_time);
  tone(beep_pin,f_min);
  delay(delay_time);
  
}

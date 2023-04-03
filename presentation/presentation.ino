
//WIFI settings to ThingSpeak Server
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid = "ZTE_2.4G_9jrwUL"; //Your Network SSID
const char* password = "Gp666Mfv"; //Your Network Password

WiFiClient client;
unsigned long myChannelNumber = 1359003; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "L19BIH8BMHEYOABN"; //Your Write API Key
const char * myReadAPIKey = "AO7O9HUB8XZ2VX19"; //Read API key

// Calibrate
const int numReadings = 10;

int inputPin = A0;              // define pin number
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int finalRead;                  // final output

//filtering
int filterValue = 0;
int windowStatus;

//newly added variables
int zeros = 0;
int finalAve = 0;
int timer = 0;
int sum = 0;

//relay control for power window
const unsigned char in1= 12; //D6
const unsigned char in2= 13; //D7

//sound
const unsigned char beep_pin = 2;
int gap=1000;

//i2C display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  // initialize serial communication with computer:
  Serial.begin(9600);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);

  // initialize LCD
  lcd.begin();
  lcd.backlight();
  
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  //initializing screen
  startUp();
  lcd.clear();
  
  //sound
  pinMode(beep_pin, OUTPUT);

  pinMode(in1, INPUT);
}

void loop() {
  //print label of values
  lcd.setCursor(1, 0);
  lcd.print("PPM Status");
  sample();
  //print corresponding values

  delay(1000);
}

void IoT(){
  ThingSpeak.writeField(myChannelNumber, 1, finalRead, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, filterValue, myWriteAPIKey);
  //ThingSpeak.writeField(myChannelNumber, 3, windowStatus, myWriteAPIKey);
  delay(1000);
}

void sample(){
  int i;
  
  for(i=0; i<2 ; i++){
    finalRead=0;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
    }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0; i<3; i++){
    finalRead=1;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
    }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<2;i++){
    finalRead=2;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<1;i++){
    finalRead=4;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<2;i++){
    finalRead=5;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<2;i++){
    finalRead=6;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<2;i++){
    finalRead=5;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<5;i++){
    finalRead=7;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<2;i++){
    finalRead=6;
    digitalRead(in1);
    if(in1==HIGH){
      danger();
    }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  for(i=0;i<3;i++){
    finalRead=7;
    digitalRead(in1);
    if(in1==HIGH){
    danger();
  }
    lcd.setCursor(1, 1);
    lcd.print(finalRead);
    filter();
    IoT();
    Serial.println(finalRead);
    delay(14000);
  }
  danger();
}

/*void Calibrate(){
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

  /*Serial.print("Actual: ");
  Serial.println(analogRead(A0));
  Serial.print("Average: ");
  Serial.println(average);
  
  //finalRead=finalRead+1;
  Serial.print("Final: ");
  Serial.println(finalRead);
  Serial.print("Timer: ");
  Serial.println(timer);
  Serial.print("Sum ");
  Serial.println(sum);
  Serial.println(" ");
  delay(1000);        // delay in between reads for stability
}
*/

void startUp(){
  int sec=0;
  for(sec=0; sec<=10; sec++){
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Initializing");
    lcd.setCursor(4, 1);
    lcd.print(10-sec);
    lcd.setCursor(7, 1);
    lcd.print("sec");
    delay(1000);
  }
  /*lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Closing");
  lcd.setCursor(7, 1);
  windowUP();
  */
}

void filter(){
  /*lcd.setCursor(13, 1);
      lcd.print(timer);
      */
  if(finalRead==0){
      cero();    //function for continuous zero readings
      
      ++zeros;
      ++timer;
      if(zeros>=300){
      ESP.restart();//microcontroller soft restart
      }
  }
  else if(finalRead>=1 && finalRead<=6){
      veryLow();
      sum = sum + finalRead;
      zeros==0;
      ++timer;
      timergang();
  }
  delay(1000);
}

//filtering
void cero(){
  lcd.setCursor(4, 1);
  lcd.print(" CLEAR  ");
  
  //data sent to thingspeak
  filterValue=0;
}

void veryLow(){
  lcd.setCursor(4, 1);
  lcd.print("VERY LOW");
  
  //data sent to thingspeak
  filterValue=1;
}

void low(){
  lcd.setCursor(4, 1);
  lcd.print("  LOW   ");
  
  //data sent to thingspeak
  filterValue=2;
}

void mid(){
  lcd.setCursor(4, 1);
  lcd.print("  MID   ");
  
  //data sent to thingspeak
  filterValue=3;
}

void high(){
  lcd.setCursor(4, 0);
  lcd.print("  HIGH   ");
  
  //data to send to thingspeak
  filterValue=4;
}

void veryHigh(){
  //print on lcd
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("VERY HIGH");
  
  //data to send to thingspeak
  filterValue=5;
}

void timergang(){
if(timer=4){
    finalAve = sum/4;
    if(finalAve>200)
    {
      danger();
    }
}
else if(timer=15){
    finalAve = sum/15;
    if(finalAve=200)
    {
      danger();
    }
}
else if(timer=60){
    finalAve = sum/60;
    if(finalAve=149)
    {
      danger();
    }
}  
else if(timer=480){
    finalAve = sum/480;
    if(finalAve=35)
    {
      danger();
    } 
}
else if(timer=1440){
    finalAve = sum/1440;
    if(finalAve=6)
    {
      danger();
    }
}
 else if(timer>=45 && timer<60){ //check if mid is critical
    finalAve = sum/(2.48*timer);
    if(finalAve>111.75 && finalAve<149)
    {
      filterValue=7;//display warning
      filter();
    }
 }
else if(timer>=420 && timer<480){ //check if low is critical
    finalAve = sum/(0.0729*timer);
    if(finalAve>111.75 && finalAve<149)
    {
      filterValue=7; //display warning
      filter();
    }
 }
else if(timer>=1390 && timer<1440){ //check if very low is critical
    finalAve = sum/(0.004167*timer);
    if(finalAve>111.75 && finalAve<149)
    {
      filterValue=7; //display warning
      filter();
    }
}
  else{
    filter();
  }
}

void danger(){
    filterValue=6;
    windowDOWN();
    sound();
    ESP.restart();
}


//relay
void windowUP(){
  //relay 1 is active
  Serial.println("Relay 1 active");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  windowStatus=0;

  delay(3000);
  pinMode(in1, INPUT);
  pinMode(in2, INPUT);
}

void windowDOWN(){
  //relay 2 is active
  Serial.println("Relay 2 active");
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  windowStatus=1;

  delay(3000);
  pinMode(in1, INPUT);
}

//sound
void sound(){
  for (int count=1;count<=10;count++)
  {
    risefall();
    noTone(beep_pin);//
    delay(gap);
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

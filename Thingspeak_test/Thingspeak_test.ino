#include <ESP8266WiFi.h>;

#include <WiFiClient.h>;

#include <ThingSpeak.h>;

const char* ssid = "ZTE_2.4G_9jrwULy"; //SSID update before commit
const char* password = "666GpMpV"; //Password update before commit
int val;
int LDRpin = A0; //LDR Pin Connected at A0 Pin
int test = 0;

WiFiClient client;
unsigned long myChannelNumber = 1359003; //Channel Number
const char * myWriteAPIKey = "L19BIH8BMHEYOABN"; //API Key

void setup()

{
  Serial.begin(9600);
  delay(10);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
}

void loop(){

  val = analogRead(LDRpin); //Read Analog values and Store in val variable
  test = digitalRead(D0);
  Serial.println(val); //Print on Serial Monitor
  delay(1000);

  ThingSpeak.writeField(myChannelNumber, 1,val, myWriteAPIKey); //Update in ThingSpeak
  ThingSpeak.writeField(myChannelNumber, 2,test, myWriteAPIKey);
  
  delay(100);
}

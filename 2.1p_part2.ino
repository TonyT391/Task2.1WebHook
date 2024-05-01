/*
  WriteMultipleFields
  
  Description: Writes values to fields 1,2,3,4 and status in a single ThingSpeak update every 20 seconds.
  
  Hardware: Arduino MKR1000
  
  !!! IMPORTANT - Modify the secrets.h file for this project with your network connection and ThingSpeak channel details. !!!
  
  Note:
  - Requires WiFi101 library version 0.15.3 or newer.
  - This example is written for a network using WPA encryption. For WEP or WPA, change the WiFi.begin() call accordingly.
  
  ThingSpeak ( https://www.thingspeak.com ) is an analytic IoT platform service that allows you to aggregate, visualize, and 
  analyze live data streams in the cloud. Visit https://www.thingspeak.com to sign up for a free account and create a channel.  
  
  Documentation for the ThingSpeak Communication Library for Arduino is in the README.md folder where the library was installed.
  See https://www.mathworks.com/help/thingspeak/index.html for the full ThingSpeak documentation.
  
  For licensing information, see the accompanying license file.
  
  Copyright 2020, The MathWorks, Inc.
*/

#include "WiFiNINA.h"
#include "secrets.h"//change
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#include "DHT.h"
#define DHTPIN A3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


char ssid[] = SECRET_SSID;   // your network SSID (name) //CHANGE
char pass[] = SECRET_PASS;   // your network password //CHANGE
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize our values
// int number1 = 0;
// int number2 = random(0,100);
// int number3 = random(0,100);
//int number4 = random(0,100);
String myStatus = "";




void setup() {
  Serial.begin(115200);      // Initialize serial 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 

  dht.begin();
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);


    float hif = dht.computeHeatIndex(f, h);
      float hic = dht.computeHeatIndex(t, h, false);


  // set the fields with the values
  ThingSpeak.setField(1, h);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, f);
 // ThingSpeak.setField(4, number4);
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  } 
    Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
  delay(10000); // Wait 10 seconds to update the channel again 
} 

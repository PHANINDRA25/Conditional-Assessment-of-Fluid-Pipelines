//#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
File DHtemp;
#define dht_dpin D1
DHT dht(dht_dpin, DHTTYPE);
//const char* ssid     = "avs";
//const char* password = "phani123";

//const char* host = "api.thingspeak.com";
//const char* privateKey = "CJWMPBG6TEMQ2EGA";
void setup() {
  Serial.println();
  Serial.println();
  
  Serial.print("Initializing SD card...");

  if (!SD.begin()) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.print("Connecting to ");
 // Serial.println(ssid);
  
//  WiFi.begin(ssid, password);
  
  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
    
  Serial.begin (9600);
  Serial.print(".");
  dht.begin();
  Serial.begin(9600);
  Serial.println("Humidity and temperature \n\n");
   Serial.println("==========================================");
  delay(700);
  }

//  Serial.println("");
  //Serial.println("WiFi connected");  
 // Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());



void loop() {
   
  float h = dht.readHumidity();
    float t = dht.readTemperature();         
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    
    DHtemp = SD.open("Temp.txt", FILE_WRITE);
    if(DHtemp){
    DHtemp.print("Current humidity = ");
    DHtemp.print(h);
    DHtemp.print("%  ");
    DHtemp.print("temperature = ");
    DHtemp.print(t); 
    DHtemp.println("C  ");
    DHtemp.close();
    Serial.println("done");
    }
    else{
      Serial.println("error opening DHtemp.txt");
    }
  //delay(800);
  //Serial.print("connecting to ");
//  Serial.println(host);
}
  // Use WiFiClient class to create TCP connections
  

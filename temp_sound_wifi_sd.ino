#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
File DHtemp;
#define dht_dpin D1
DHT dht(dht_dpin, DHTTYPE);
int pinSignal= A0; // pin connected to pin O module sound sensor
long Sound_signal;    // Store the value read Sound Sensor
File LMsound;
const char* ssid     = "ABHI";
const char* password = "abhinay123";

const char* host = "api.thingspeak.com";
const char* privateKey1 = "CJWMPBG6TEMQ2EGA";   //temperature
const char* privateKey2 = "5LZ359N8U1XZ4USZ";  //sound

int num_Measure = 128 ; // Set the number of measurements      
long sum = 0 ; // Store the total value of n measurements   
long level = 0 ; // Store the average value   

void setup() {
  Serial.println();
  Serial.println();
  
  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  Serial.begin (9600);
  Serial.print(".");
  dht.begin();
  Serial.begin(9600);
     pinMode (pinSignal, INPUT); // Set the signal pin as input  
  Serial.println("Humidity and temperature \n\n");
   Serial.println("==========================================");
  delay(700);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}
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

    
    for ( int i = 0 ; i <num_Measure; i ++)  
  {  
   Sound_signal = analogRead (pinSignal);  
    sum =sum + Sound_signal;  
  }  
 
  level = sum / num_Measure; // Calculate the average value   
  Serial.print("Sound Level: ");
  Serial.println (level);  
  
    LMsound = SD.open("Sound.txt", FILE_WRITE);
    if(LMsound){
      LMsound.print("Sound Level: ");
      LMsound.println (level);
      LMsound.close();
    Serial.println("done");
    }
    else{
      Serial.println("error opening LMsound.txt");
    }
    
  sum=0;
  delay(800);
  Serial.print("connecting to ");
  Serial.println(host);
  
   //Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
   //We now create a URI for the request
  String url = "/update";
  url += "?api_key=";
  url += privateKey1;
  url += "&field1=";
  url += h;
  url += "&field2=";
  url += t;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
//   This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  WiFiClient client2;
//  const int httpPort = 80;
  if (!client2.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
//   We now create a URI for the request
  String urli = "/update";
  urli += "?api_key=";
  urli += privateKey2;
  urli += "&field1=";
  urli += level;
  
   Serial.print("Requesting URL: ");
  Serial.println(urli);
  
  // This will send the request to the server
  client.print(String("GET ") + urli + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line2 = client.readStringUntil('\r');
    Serial.print(line2);
  }
  
  
  Serial.println();
  Serial.println("closing connection");
}

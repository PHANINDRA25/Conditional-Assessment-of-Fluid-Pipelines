#include <ESP8266WiFi.h>
#include <SPI.h>
#include <SD.h>

int pinSignal= A0; // pin connected to pin O module sound sensor
long Sound_signal;    // Store the value read Sound Sensor
const char* ssid     = "connected";
const char* password = "8333008282";
File LMsound;
const char* host = "api.thingspeak.com";
const char* privateKey = "5LZ359N8U1XZ4USZ";

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
  Serial.begin(9600);

   pinMode (pinSignal, INPUT); // Set the signal pin as input   
  Serial.begin (9600); 
 
   Serial.println("==========================================");
  delay(700);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


}
void loop() {



  Serial.print("connecting to ");
  Serial.println(host);

for ( int i = 0 ; i <num_Measure; i ++)  
  {  
   Sound_signal = analogRead (pinSignal);  
    sum =sum + Sound_signal;  
  }  
 
  level = sum / num_Measure; // Calculate the average value   
  Serial.print("Sound Level: ");
  Serial.println (level);  
  
    LMsound = SD.open("Soundball.txt", FILE_WRITE);
    if(LMsound){
      LMsound.print("Sound Level: ");
      LMsound.println (level);
      LMsound.close();
    Serial.println("done");
    }
    else{
      Serial.println("error opening LMsound.txt");
    }
  sum = 0 ; // Reset the sum of the measurement values  
  delay(200);

  
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
 String url = "/update";
  url += "?api_key=";
  url += privateKey;
  url += "&field1=";
  url += level;

  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(1000);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}

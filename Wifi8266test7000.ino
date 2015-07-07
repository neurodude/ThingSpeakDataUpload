#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Must be unique on local network

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "DX5T2ORNMD70MEV8";
const int updateThingSpeakInterval = 16 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;
int buttonState = 0;


char* ssid = "OrangeSpruce";     //  your network SSID (name) 
char* pass = "JzWsUmjrPZ";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Create an instance of the server
// specify the port to listen on as an argument
WiFiClient client;
int i = 0;
double lightVal = 0.0;
void setup() {
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(12, INPUT);
  Serial.begin(115200);
  delay(10);

  // prepare GPIO2
 
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {

    digitalWrite(5,HIGH);
    delay(500);
    Serial.print(".");
    digitalWrite(5,LOW);    
    
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
void loop(){


  
  // Update ThingSpeak
    lightVal = analogRead(A0);
    String timeVal = "&field1=";
    String lightLevel = "&field2=";
    buttonState = digitalRead(12);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    //digitalWrite(ledPin, HIGH); 
    updateThingSpeak(timeVal+i+lightLevel+lightVal); 
  } 
  else {
    // turn LED off:
    digitalWrite(4, LOW); 
  }
      
    i++;
    
  
  
  lastConnected = client.connected();
}

void updateThingSpeak(String tsData)
{ 
  if (client.connect(thingSpeakAddress, 80))
  {         
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    digitalWrite(4,HIGH);
    delay(500);
    digitalWrite(4,LOW);
    
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}
 

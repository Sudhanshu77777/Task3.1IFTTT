#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>
#include "Secrets.h"

char ssid [] = SECRET_SSID ;
char pass [] = SECRET_PASS;

WiFiClient client;

BH1750 lightMeter; // Create an instance of the BH1750 light sensor library const 
int ledCheck = 2;

char HOST_NAME[] = "maker.ifttt.com"; // Website
String PATH_NAME_1 = "https://maker.ifttt.com/trigger/light/with/key/moHh8LQ4zQ_kAwx4elV-gxXx0K2-GWxTZCerU8CpR-h"; // Key
String PATH_NAME_2 = "https://maker.ifttt.com/trigger/NO_SUNLIGHT/with/key/moHh8LQ4zQ_kAwx4elV-gxXx0K2-GWxTZCerU8CpR-h"; //Key
String queryString;

void setup()
{
  WiFi.begin(ssid, pass);
  Serial.begin(9600);
  
  while (!Serial);

  Serial.println("Attempting to connect to WiFi...");
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(ssid, pass);
    delay(2000);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");

  pinMode(ledCheck, OUTPUT);
  Wire.begin();
  lightMeter.begin();
  Serial.println(F("BH1750 Test begin"));
}

void loop() 
{
  float light_intensity = lightMeter.readLightLevel();
  Serial.print(": ");
  Serial.print(light_intensity);
  Serial.println(" LUX");

  queryString = "?value1="+ String(light_intensity); 

  if (light_intensity >= 300) 
  {
    triggering_data_to_Webhook_1();
  } 
  else if (light_intensity < 50) 
  {
    triggering_data_to_Webhook_2();
  }
  delay(60000); // Wait for 1 minute before next reading
}

void triggering_data_to_Webhook_1() 
{ 
  if (client.connect("maker.ifttt.com", 80)) 
  {
    
    client.println("GET " + PATH_NAME_1 + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
  }
  client.stop();
  Serial.println();
}

void triggering_data_to_Webhook_2()
{
   if (client.connect("maker.ifttt.com", 80)) 
  {
    client.println("GET " + PATH_NAME_2 + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while (client.connected()) {
      if (client.available()) {
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }
  }
  client.stop();
  Serial.println();
}

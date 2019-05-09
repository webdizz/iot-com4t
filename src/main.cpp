#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU");
  Serial.println("Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Idle...");
   // wait for a second
  delay(1000);

} 
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Ticker.h>

// GPIO5
#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature tempSensor(&oneWire);
DeviceAddress insideThermometer;

float recentTemperature = 0.00;

const char *host = "api.github.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char fingerprint[] PROGMEM = "5F F1 60 31 09 04 3E F2 90 D2 B0 8A 50 38 04 E8 37 9F BC 76";

void setUpThermoSensor()
{
  tempSensor.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(tempSensor.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // search for thermometer
  oneWire.reset_search();
  if (!oneWire.search(insideThermometer))
  {
    Serial.println("Unable to find address for insideThermometer");
  }
  else
  {
    // set the resolution to 9 bit
    tempSensor.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  }
}

void retrieveTemperature()
{
  tempSensor.requestTemperatures();
  float tempC = tempSensor.getTempC(insideThermometer);
  if (tempC != recentTemperature)
  {
    recentTemperature = tempC;
    Serial.printf("Current Temperature is %.1fC\n", recentTemperature);
  }
}

void connectToWiFi()
{
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  // client.setFingerprint(fingerprint);

  // if (!client.connect(host, httpsPort))
  // {
  //   Serial.println("connection failed");
  //   return;
  // }

  // String url = "/repos/esp8266/Arduino/commits/master/status";
  // Serial.print("requesting URL: ");
  // Serial.println(url);

  // client.print(String("GET ") + url + " HTTP/1.1\r\n" +
  //              "Host: " + host + "\r\n" +
  //              "User-Agent: BuildFailureDetectorESP8266\r\n" +
  //              "Connection: close\r\n\r\n");

  // Serial.println("request sent");
  // while (client.connected())
  // {
  //   String line = client.readStringUntil('\n');
  //   if (line == "\r")
  //   {
  //     Serial.println("headers received");
  //     break;
  //   }
  // }
  // String line = client.readStringUntil('\n');
  // if (line.startsWith("{\"state\":\"success\""))
  // {
  //   Serial.println("esp8266/Arduino CI successfull!");
  // }
  // else
  // {
  //   Serial.println("esp8266/Arduino CI has failed");
  // }

  Serial.println("closing connection");
}

Ticker temperatureTicker(retrieveTemperature, 5000);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // setup for temperature retrieving
  setUpThermoSensor();
  temperatureTicker.start();

  // connectToWiFi();
}

void loop()
{
  // put your main code here, to run repeatedly:

  //retrieve temperature
  temperatureTicker.update();
}
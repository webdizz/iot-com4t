#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"
#include "WiFiCfg.h"
#include "FS.h"

#include <DHT.h>
#include <Ticker.h>

struct WiFiConnection wiFiConnection;

WebThingAdapter *adapter;

#if defined(LED_BUILTIN)
const int ledPin = LED_BUILTIN;
#else
const int ledPin = 13; // manually configure LED pin
#endif

#define DHTPIN 13
#define DHTTYPE           DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);

const char* deviceTypes[] = {"MultiSensor", "Sensor", nullptr};
ThingDevice dht22("dht22", "Temperature and Humidity sensor", deviceTypes);
ThingProperty temperatureProperty("temperature", "Temperature", NUMBER, "Temperature");
ThingProperty humidityProperty("humidity", "Humidity", NUMBER, "Humidity");

void retrieveAirCondition();
Ticker temperatureTicker(retrieveAirCondition, 5000);

void setup()
{
  connect(ledPin, wiFiConnection);

  // setup 
  adapter = new WebThingAdapter("IoT Comfort", WiFi.localIP());
  dht22.addProperty(&temperatureProperty);
  dht22.addProperty(&humidityProperty);
  adapter->addDevice(&dht22);
  adapter->begin();

  dht.begin();
  delay(1000);
  temperatureTicker.start();
}

void retrieveAirCondition()
{
  if(dht.read())
  {
    int humidity = (int)dht.readHumidity();
    int temperature = (int)dht.readTemperature();
    
    ThingPropertyValue humidityValue;
    humidityValue.number = humidity;
    humidityProperty.setValue(humidityValue);

    ThingPropertyValue temperatureValue;
    temperatureValue.number = temperature;
    temperatureProperty.setValue(temperatureValue);
  }
}

void loop()
{
  adapter->update();
  temperatureTicker.update();
}
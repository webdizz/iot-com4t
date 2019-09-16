#include <WiFiCfg.h>

#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

void populateWiFiConnection(WiFiConnection &wiFiConnection, JsonObject &json)
{
    // Copy values from the JSON document
    wiFiConnection.ssid = json["wifi"]["ssid"].as<char *>();
    wiFiConnection.password = json["wifi"]["password"].as<char *>();
}

bool loadConfig(WiFiConnection &wiFiConnection)
{
    const char *filename = "/config.json";
    File configFile = SPIFFS.open(filename, "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    // Deserialize the JSON document
    StaticJsonBuffer<512> jb;
    JsonObject &json = jb.parseObject(configFile);

    if (!json.success())
    {
        Serial.println(F("Failed to read configuration file, using default configuration"));
    }

    populateWiFiConnection(wiFiConnection, json);

    return true;
}

void connect(const int ledPin, WiFiConnection &wiFiConnection)
{
    Serial.begin(115200);

    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }
    if (!loadConfig(wiFiConnection))
    {
        Serial.println("Failed to load config");
    }
    else
    {
        Serial.println("Config loaded");
    }
    delay(500);

    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);

    Serial.println("");
    Serial.print("Connecting to \"");
    Serial.print(wiFiConnection.ssid);
    Serial.println("\"");

    #if defined(ESP8266) || defined(ESP32)
    WiFi.mode(WIFI_STA);
    #endif
    WiFi.begin(wiFiConnection.ssid, wiFiConnection.password);

    // Wait for connection
    bool blink = true;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        digitalWrite(ledPin, blink ? LOW : HIGH);
        Serial.println("Is about to connect to WiFi");
        blink = !blink;
    }
    digitalWrite(ledPin, HIGH);
    delay(500);
}
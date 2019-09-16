#include <ESP8266WiFi.h>

struct WiFiConnection
{
    const char *ssid;
    const char *password;
};

bool loadConfig(WiFiConnection &wiFiConnection);

void connect(const int ledPin, WiFiConnection &wiFiConnection);
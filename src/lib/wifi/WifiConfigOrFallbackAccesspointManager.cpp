#include "WifiConfigOrFallbackAccesspointManager.h"

#include <HardwareSerial.h>
#include <IPAddress.h>
#include <WiFiManager.h>

WifiConfigOrFallbackAccesspointManager::WifiConfigOrFallbackAccesspointManager()
{
    WiFiManager wifiManager;
    wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1), IPAddress(255, 255, 255, 0));
    wifiManager.autoConnect();
    Serial.println("setup: wifi connected");
}
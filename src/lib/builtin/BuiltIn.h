#pragma once

#include <Arduino.h>

struct BuiltIn
{
    static void ledOn()
    {
        digitalWrite(LED_BUILTIN, 0);
    }

    static void ledOff()
    {
        digitalWrite(LED_BUILTIN, 1);
    }

    static void ledToggle()
    {
        digitalWrite(LED_BUILTIN, static_cast<uint8_t>(0x01 & ~digitalRead(LED_BUILTIN)));
    }

    static void setup()
    {
        pinMode(LED_BUILTIN, OUTPUT);
        ledOff();
    }
} ;
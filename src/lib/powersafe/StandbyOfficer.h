#pragma once

#include <Arduino.h>
#include <elapsedMillis.h>

#include <BuiltIn.h>
#include <PixelRing.h>

struct StandbyOfficer
{
    StandbyOfficer(PixelRing &pixel_ring, unsigned long seconds_idle_before_standby = 60, const String &name = "scotty")
            :
            seconds_idle_before_standby(seconds_idle_before_standby),
            pixel_ring(pixel_ring),
            name(name)
    {}

    void process()
    {
        if (!enabled)
            return;

        if (elapsed_seconds >= seconds_idle_before_standby)
        {
            Serial.print("StandbyOfficer::process: officer ");
            Serial.print(name.c_str());
            Serial.print(" sends device to sleep after ");
            Serial.print(elapsed_seconds);
            Serial.println(" seconds");

            pixel_ring.off();
            pixel_ring.process(PixelRing::SceneMode::White);
            for (uint8_t i = 0; i < 10; i++)
            {
                pixel_ring.on();
                pixel_ring.nextScene();
                pixel_ring.process();
                delay(100);
                pixel_ring.off();
                pixel_ring.process();
                delay(100);
            }
            pixel_ring.process(PixelRing::SceneMode::White);
            pixel_ring.off();
            disable();
            BuiltIn::ledOff();
            ESP.deepSleep(ESP.deepSleepMax());
        }
    }

    void enable()
    {
        Serial.print("StandbyOfficer::enable: officer ");
        Serial.println(name.c_str());
        elapsed_seconds = 0;
        enabled = true;
    }

    void disable()
    {
        Serial.print("StandbyOfficer::disable: officer ");
        Serial.println(name.c_str());
        enabled = false;
    }

    void reset()
    {
        Serial.print("StandbyOfficer::reset: officer ");
        Serial.println(name.c_str());
        elapsed_seconds = 0;
    }

private:
    bool enabled{false};
    elapsedSeconds elapsed_seconds{std::numeric_limits<unsigned long>::max()};
    unsigned long seconds_idle_before_standby;
    PixelRing &pixel_ring;
    const String name;
};

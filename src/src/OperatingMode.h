#pragma once

#include <HardwareSerial.h>

struct OperatingMode
{
    enum class Mode
    {
        Normal,
        SwitchToWifi,
        Wifi
    };

    void setMode(Mode new_mode)
    {
        Serial.print("OperatingMode::setMode: switching from ");
        Serial.println(std::underlying_type<Mode>::type(mode));
        mode = new_mode;
        Serial.print(" to ");
        Serial.println(std::underlying_type<Mode>::type(mode));
    }

    bool operator==(Mode other_mode)
    {
        return this->mode == other_mode;
    }


private:
    Mode mode{Mode::Normal};
};
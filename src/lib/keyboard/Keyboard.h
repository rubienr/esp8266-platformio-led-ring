#pragma once

#include <elapsedMillis.h>
#include <Adafruit_MPR121.h>

class KeyEventReceiver;

class Keyboard
{
    uint16_t last_touched_flags = 0;
    Adafruit_MPR121 cap;
    elapsedMillis key_repeated_time_elapsed {0};
    elapsedMillis key_double_pressed_time_elapsed {0};
    KeyEventReceiver *key_event_receiver = {nullptr};

public:

    Keyboard(KeyEventReceiver *receiver = nullptr);
    void setup();
    void process();
    bool setEventReceiver(KeyEventReceiver *receiver);
};

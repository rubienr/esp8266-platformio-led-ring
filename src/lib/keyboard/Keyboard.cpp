#include "Keyboard.h"

#include <Wire.h>

#include "KeyEventReceiver.h"

Keyboard::Keyboard(KeyEventReceiver *receiver)
{
    setEventReceiver(receiver);
}

void Keyboard::setup()
{
    Serial.println("Keyboard::setup");
    uint8_t retry = 32;

    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    while (!cap.begin(0x5B))
    {
        Serial.println("Keyboard::setup: device not found; retry");
        delay(125);
        if (--retry <= 0)
        {
            Serial.println("Keyboard::setup: device not found; gave up");
            return;
        }
    }
}

void Keyboard::process()
{
    uint16_t touched_flags = cap.touched();

    auto is_touched = [&](uint8_t bit_nr)
    {
        return touched_flags & _BV(bit_nr);
    };

    auto was_touched = [&](uint8_t bit_nr)
    {
        return last_touched_flags & _BV(bit_nr);
    };

    static uint16_t num_key_repeats = 0;
    uint8_t num_keys = 12;
    for (uint8_t key_nr = 0; key_nr < num_keys; key_nr++)
    {
        KeyEvent event;
        event.key = static_cast<KeyEvent::Key>(key_nr);

        if (is_touched(key_nr) && !was_touched(key_nr))
        {
            Serial.print("Keyboard::process: key ");
            Serial.print(key_nr);

            if (key_repeated_time_elapsed <= 250)
            {
                Serial.println(" double-pressed ");
                event.type = KeyEvent::Type::DoublePressed;
            } else
            {
                Serial.println(" pressed ");
                event.type = KeyEvent::Type::Pressed;
            }

            num_key_repeats = 0;
            key_repeated_time_elapsed = 0;
            key_double_pressed_time_elapsed = 0;

        } else if (was_touched(key_nr) && is_touched(key_nr))
        {
            if (key_repeated_time_elapsed >= 125)
            {
                event.type = KeyEvent::Type::Repeated;
                Serial.print("Keyboard::process: key ");
                Serial.print(key_nr);
                Serial.println(" repeated");
                ++num_key_repeats;
                key_repeated_time_elapsed = 0;
            }
        } else if (was_touched(key_nr) && !is_touched(key_nr))
        {
            event.type = KeyEvent::Type::Released;
            num_key_repeats = 0;
            Serial.print("Keyboard::process: key ");
            Serial.print(key_nr);
            Serial.println(" released");
        }

        event.repeated = num_key_repeats;

        if (key_event_receiver != nullptr && event.type != KeyEvent::Type::None)
        {
            key_event_receiver->take(event);
        }
    }

    last_touched_flags = touched_flags;
}

bool Keyboard::setEventReceiver(KeyEventReceiver *receiver)
{
    if (key_event_receiver != receiver)
    {
        key_event_receiver = receiver;
        return true;
    }
    return false;
}

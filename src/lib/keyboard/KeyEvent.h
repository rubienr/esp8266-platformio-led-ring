#pragma once

#include <stdint.h>

struct KeyEvent
{
    enum class Key : uint8_t
    {
        Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, Key11, None
    };

    enum class Type : uint8_t
    {
        Pressed = 1,
        Released = 2,
        Repeated = 4,
        DoublePressed = 8,
        None = 16
    };

    Key key{Key::None};
    Type type{Type::None};
    uint16_t repeated{0};
};

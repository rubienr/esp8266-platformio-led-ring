#pragma once

#include "KeyEvent.h"

class KeyEventReceiver
{
public:
    virtual bool take(const KeyEvent e) = 0;

    virtual ~KeyEventReceiver() = default;
};

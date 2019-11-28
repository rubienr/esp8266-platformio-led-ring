#pragma once

#include <Arduino.h>

#include <PixelRing.h>
#include <StandbyOfficer.h>
#include <KeyEventReceiver.h>
#include <Keyboard.h>

#include "OperatingMode.h"

class KeyEventHandler : public KeyEventReceiver
{
    PixelRing &receiver;
    OperatingMode &operating_mode;
    StandbyOfficer &standby_officer_on_long_term;
    StandbyOfficer &standby_officer_on_lights_off;

public:

    explicit KeyEventHandler(PixelRing &receiver, OperatingMode &operating_mode, StandbyOfficer &standby_officer,
                             StandbyOfficer &standby_officer_on_lights_off) :
            receiver(receiver),
            operating_mode(operating_mode),
            standby_officer_on_long_term(standby_officer),
            standby_officer_on_lights_off(standby_officer_on_lights_off)
    {
    }

    bool take(const KeyEvent e) override
    {
        Serial.print("KeyEventHandler::take: key=");
        Serial.print(std::underlying_type<KeyEvent::Key>::type(e.key));
        Serial.print(" type=");
        Serial.print(std::underlying_type<KeyEvent::Type>::type(e.type));
        Serial.print(" repeated=");
        Serial.println(e.repeated);

        bool consumed = false;

        if (!consumed && e.type == KeyEvent::Type::DoublePressed)
        {
            switch (e.key)
            {
                case KeyEvent::Key::Key1:
                case KeyEvent::Key::Key3:
                case KeyEvent::Key::Key4:
                case KeyEvent::Key::Key5:
                case KeyEvent::Key::Key6:
                case KeyEvent::Key::Key7:
                    break;
                case KeyEvent::Key::Key0:
                case KeyEvent::Key::Key2:
                case KeyEvent::Key::Key8:
                case KeyEvent::Key::Key10:
                    receiver.maxBrightness();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key9:
                case KeyEvent::Key::Key11:
                    ESP.restart();
                    break;
                case KeyEvent::Key::None:
                    break;
            }
        }

        if (!consumed && e.type == KeyEvent::Type::Repeated)
        {
            const uint8_t long_press = 6;
            const uint8_t extra_long_press = 2 * long_press;

            switch (e.key)
            {
                case KeyEvent::Key::Key1:
                case KeyEvent::Key::Key3:
                case KeyEvent::Key::Key4:
                    break;
                case KeyEvent::Key::Key5:
                    if (e.repeated >= extra_long_press)
                    {
                        operating_mode.setMode(OperatingMode::Mode::SwitchToWifi);
                        consumed = true;
                    }
                    break;
                case KeyEvent::Key::Key6:
                case KeyEvent::Key::Key7:
                    break;
                case KeyEvent::Key::Key0:
                case KeyEvent::Key::Key2:
                case KeyEvent::Key::Key8:
                case KeyEvent::Key::Key10:
                    if (e.repeated >= long_press)
                    {
                        receiver.fullWidth();
                        receiver.on();
                        consumed = true;
                    }
                    break;
                case KeyEvent::Key::Key9:
                case KeyEvent::Key::Key11:
                case KeyEvent::Key::None:
                    break;
            }
        }

        if (!consumed && (e.type == KeyEvent::Type::Pressed || e.type == KeyEvent::Type::Repeated))
        {
            standby_officer_on_long_term.reset();
            standby_officer_on_lights_off.disable();

            switch (e.key)
            {
                case KeyEvent::Key::Key8:
                    receiver.on();
                    receiver.process(PixelRing::SceneMode::White);
                    consumed = true;
                    break;
                case KeyEvent::Key::Key0:
                    receiver.on();
                    receiver.process(PixelRing::SceneMode::Red);
                    consumed = true;
                    break;
                case KeyEvent::Key::Key10:
                    receiver.on();
                    receiver.process(PixelRing::SceneMode::Green);
                    consumed = true;
                    break;
                case KeyEvent::Key::Key2:
                    receiver.process(PixelRing::SceneMode::Blue);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key3:
                    receiver.incrementBrightness(20);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key7:
                    receiver.incrementBrightness(-20);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key11:
                    if (!receiver.toggleOnOff())
                    {
                        standby_officer_on_lights_off.enable();
                    }
                    consumed = true;
                    break;
                case KeyEvent::Key::Key1:
                    receiver.shift(1);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key9:
                    receiver.shift(-1);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key4:
                    receiver.incrementWidth(1);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key6:
                    receiver.incrementWidth(-1);
                    receiver.on();
                    consumed = true;
                    break;
                case KeyEvent::Key::Key5:
                    receiver.nextScene();
                    receiver.on();
                    break;
                case KeyEvent::Key::None:
                    break;
            }
        }
        return
                consumed;
    }

};

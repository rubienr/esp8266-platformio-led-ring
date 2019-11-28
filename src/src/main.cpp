#include <Arduino.h>
#include <elapsedMillis.h>

#include <PixelRing.h>
#include <KeyEventReceiver.h>
#include <Keyboard.h>
#include <StandbyOfficer.h>
#include <WifiConfigOrFallbackAccesspointManager.h>
#include <WebService.h>

#include "../lib/builtin/BuiltIn.h"
#include "KeyEventHandler.h"

ADC_MODE(ADC_VCC);

struct Resources
{
    struct EarlyInitializer
    {
        EarlyInitializer()
        {
            BuiltIn::setup();
            Serial.begin(9600);
            while (!Serial)
                delay(10);

            pinMode(LED_BUILTIN, OUTPUT);
            digitalWrite(LED_BUILTIN, LOW);
        }
    } early_init;

    BuiltIn build_in;
    OperatingMode operating_mode;
    PixelRing strip;
    StandbyOfficer standby_officer_on_long_term{strip, 60 * 60, "long term"};
    StandbyOfficer standby_officer_on_lights_off{strip, 20 * 60, "short term"};
    KeyEventHandler event_handler{strip,
                                  operating_mode,
                                  standby_officer_on_long_term,
                                  standby_officer_on_lights_off};
    Keyboard keyboard{&event_handler};
    WebService web_service;

    void setup()
    {
        Serial.println("setup");
        strip.setup();
        keyboard.setup();
        standby_officer_on_long_term.enable();
        Serial.println("process");
    }

    void process()
    {
        if (operating_mode == OperatingMode::Mode::Normal)
        {
            processNormalMode();
        } else if (operating_mode == OperatingMode::Mode::SwitchToWifi)
        {
            switchToWifiMode();
        } else if (operating_mode == OperatingMode::Mode::Wifi)
        {
            processWifiMode();
        } else
        {
            processNormalMode();
        }
    }

private:
    void processNormalMode()
    {
        standby_officer_on_lights_off.process();
        standby_officer_on_long_term.process();
        strip.process();
        keyboard.process();
    }

    void switchToWifiMode()
    {
        strip.on();
        strip.process(PixelRing::SceneMode::Rainbow);
        WifiConfigOrFallbackAccesspointManager do_configure_or_open_fallback_ap();
        web_service.init();
        operating_mode.setMode(OperatingMode::Mode::Wifi);
    }

    void processWifiMode()
    {
        web_service.handleClient();
    }
} r;

void setup()
{ r.setup(); }

void loop()
{ r.process(); }

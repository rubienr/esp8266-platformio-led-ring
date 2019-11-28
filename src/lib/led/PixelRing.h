#pragma once

#include <Adafruit_NeoPixel.h>
#include <elapsedMillis.h>
#include <Adafruit_MPR121.h>

#include <KeyEvent.h>

class PixelRing
{
public:
    enum class SceneMode
    {
        White,
        Red,
        Green,
        Blue,
        TheaterChaseWhite,
        TheaterChaseRed,
        TheaterChaseBlue,
        TheaterChaseRainbow,
        Rainbow,
        Off,
        None // does not touch but maintains the previous state
    };

    void setup();

    void process(SceneMode scene_mode = SceneMode::None);

    //! Increments the brightemess by maximum +/-20 %
    //! \param increment percentage to in-/decrement
    void incrementBrightness(int8_t increment);

    void maxBrightness();

    //! toggles strip on and off
    //! \return true if strip is toggled on
    bool toggleOnOff();

    void off();

    void on();

    //! Increments the arc by maximum +/- strip.numPixels()
    //! \param pixels number of pixels to in-/decrement the arc width
    void incrementWidth(int8_t pixels);

    void fullWidth();

    //! Shifts (rotates) the arc.
    //! \param pixels number of pixels to shift for-/backward
    void shift(int8_t pixels);

    //! Scrolls to the next scene mode: White, Red, ..., Rainbow, White, ... etc.
    void nextScene();

private:
    //! Arc based abstraction of the strip.
    struct ArcBasedView
    {
        ArcBasedView(Adafruit_NeoPixel &strip);

        void process(uint32_t color);

        void process();

        void rotate(int8_t pixels = 1);

        void incrementArc(int8_t pixels = 1);

        void fullWidth();

    private:
        void incrementArcByOne(bool do_increment);

        Adafruit_NeoPixel &strip;
        uint32_t color{0};
        uint8_t begin : 4;
        uint8_t end : 4;

        //! 4-bit pixel iterator with intended over-/underflow
        uint8_t pixel_iterator : 4;
        //! toggle bit to ensures alternate access (left, right
        uint8_t toggle : 1;
    };

    uint32_t overrideColorChannelBrightness(uint8_t color);
    uint32_t overrideColorBrightness(uint32_t color);

    //! Puts the given color on the whole strip wrt. to the current brightness.
    //! \param color the color on strip
    void colorWipe(uint32_t color);


    void theaterChase(uint32_t color, uint16_t wait_ms);

    void rainbow(uint16_t wait_ms);

    void theaterChaseRainbow(uint16_t wait_ms);

    using Strip = Adafruit_NeoPixel;
    Strip strip{16, D4, (NEO_GRB) + NEO_KHZ800};

    //! 0-100 [%]
    uint8_t brightness{100};
    //! 0-1 (on, off)
    uint8_t brightness_override{0};

    SceneMode last_scene_mode = {SceneMode::Rainbow};
    //! timer to measure elapsed time in [ms] since set to 0
    elapsedMillis time_elapsed{0};

    //! arc based abstraction of the strip
    ArcBasedView arc_view{strip};
};



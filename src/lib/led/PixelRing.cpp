#include "PixelRing.h"

void PixelRing::setup()
{
    Serial.println("PixelRing::setup");
    strip.begin();
    strip.show();
}

void PixelRing::process(PixelRing::SceneMode scene_mode)
{
    last_scene_mode = (scene_mode == SceneMode::None) ? last_scene_mode : scene_mode;
    switch (last_scene_mode)
    {
        case SceneMode::Off:
            colorWipe(Strip::Color(0, 0, 0));
            break;
        case SceneMode::Red:
            arc_view.process(overrideColorBrightness(Strip::Color(255, 0, 0)));
            break;
        case SceneMode::Green:
            arc_view.process(overrideColorBrightness(Strip::Color(0, 255, 0)));
            break;
        case SceneMode::Blue:
            arc_view.process(overrideColorBrightness(Strip::Color(0, 0, 255)));
            break;
        case SceneMode::White:
            arc_view.process(overrideColorBrightness(Strip::Color(255, 255, 255)));
            break;
        case SceneMode::TheaterChaseWhite:
            theaterChase(overrideColorBrightness(Strip::Color(127, 127, 127)), 50);
            break;
        case SceneMode::TheaterChaseRed:
            theaterChase(overrideColorBrightness(Strip::Color(127, 0, 0)), 50);
            break;
        case SceneMode::TheaterChaseBlue:
            theaterChase(overrideColorBrightness(Strip::Color(0, 0, 127)), 50);
            break;
        case SceneMode::Rainbow:
            rainbow(10);
            break;
        case SceneMode::TheaterChaseRainbow:
            theaterChaseRainbow(50);
            break;
        case SceneMode::None:
            break;
    }
}

void PixelRing::incrementBrightness(int8_t increment)
{
    const int8_t max_step = 20;
    auto cap = [](int8_t &value, int8_t min, int8_t max)
    {
        value = (value > max) ? max : value;
        value = (value < min) ? min : value;
        return value;
    };

    cap(increment, -max_step, max_step);

    int8_t new_brightness = brightness;
    new_brightness += increment;
    cap(new_brightness, 5, 100);

    brightness = static_cast<uint8_t >(new_brightness);
    Serial.print("PixelRing::incrementBrightness: ");
    Serial.println(brightness);
}

void PixelRing::maxBrightness()
{
    brightness = 100;
}

uint8_t PixelRing::overrideColorChannelBrightness(uint8_t color_value)
{
    uint16_t color = ((uint16_t) brightness_override * (uint16_t) brightness * (uint16_t) color_value) / (uint16_t) 100;
    color = color > 255 ? 255 : color;
    return static_cast<uint8_t>(color);
}

uint32_t PixelRing::overrideColorBrightness(uint32_t color)
{
    uint8_t r = static_cast<uint8_t>((color & 0x00ff0000) >> 16);
    uint8_t g = static_cast<uint8_t>((color & 0x0000ff00) >> 8);
    uint8_t b = static_cast<uint8_t>((color & 0x000000ff));

    return Strip::Color(
            overrideColorChannelBrightness(r),
            overrideColorChannelBrightness(g),
            overrideColorChannelBrightness(b));
}

void PixelRing::colorWipe(uint32_t color)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
    }
}

void PixelRing::theaterChase(uint32_t color, uint16_t wait_ms)
{
    if (time_elapsed < wait_ms)
        return;
    time_elapsed = 0;

    static uint16_t a = 0, a_max = 10; // outer loop
    static uint16_t b = 0, b_max = 3; // inner loop

    {
        strip.clear();         //   Set all pixels in RAM to 0 (off)
        // 'c' counts up from 'b' to end of strip in steps of 3...
        for (uint16_t c = b; c < strip.numPixels(); c += 3)
        {
            strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        strip.show(); // Update strip with new contents
    }

    b++;
    if (b >= b_max)
    {
        a++;
        b = 0;
    }

    if (a >= a_max)
        a = 0;
}


void PixelRing::rainbow(uint16_t wait_ms)
{
    if (time_elapsed < wait_ms)
        return;
    time_elapsed = 0;

    // Hue of first pixel runs 3 complete loops through the color wheel.
    // Color wheel has a range of 65536 but it's OK if we roll over, so
    // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
    // means we'll make 3*65536/256 = 768 passes through this outer process:
    static uint32_t firstPixelHue = 0, firstPixelHue_max = 3 * 65536;

    {
        for (uint16_t i = 0; i < strip.numPixels(); i++)
        {
            // For each pixel in strip...
            // Offset pixel hue by an amount to make one full revolution of the
            // color wheel (range of 65536) along the length of the strip
            // (strip.numPixels() steps):
            uint16_t pixelHue = static_cast<uint16_t>(firstPixelHue + (i * 65536L / strip.numPixels()));
            // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
            // optionally add saturation and value (brightness) (each 0 to 255).
            // Here we're using just the single-argument hue variant. The result
            // is passed through strip.gamma32() to provide 'truer' colors
            // before assigning to each pixel:
            uint32_t color = overrideColorBrightness(Strip::gamma32(Strip::ColorHSV(pixelHue)));
            strip.setPixelColor(i, color);
        }
        strip.show(); // Update strip with new contents
        //delay(wait);  // Pause for a moment
    }

    firstPixelHue += 256;
    if (firstPixelHue > firstPixelHue_max)
        firstPixelHue = 0;

}

void PixelRing::theaterChaseRainbow(uint16_t wait_ms)
{
    if (time_elapsed < wait_ms)
        return;
    time_elapsed = 0;

    static uint16_t a = 0, a_max = 30; // outer loop
    static uint16_t b = 0, b_max = 3; // inner loop

    {
        static uint16_t firstPixelHue = 0;     // First pixel starts at red (hue 0)
        strip.clear();         //   Set all pixels in RAM to 0 (off)
        // 'c' counts up from 'b' to end of strip in increments of 3...
        for (uint16_t c = b; c < strip.numPixels(); c += 3)
        {
            // hue of pixel 'c' is offset by an amount to make one full
            // revolution of the color wheel (range 65536) along the length
            // of the strip (strip.numPixels() steps):
            uint16_t hue = static_cast<uint16_t>(firstPixelHue + c * 65536 / strip.numPixels());
            uint32_t color = overrideColorBrightness(Strip::gamma32(Strip::ColorHSV(hue))); // hue -> RGB
            strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        }
        strip.show();                // Update strip with new contents
        firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }

    b++;
    if (b >= b_max)
    {
        a++;
        b = 0;
    }

    if (a >= a_max)
        a = 0;
}

bool PixelRing::toggleOnOff()
{
    if (brightness_override == 1)
    {
        off();
    } else
    {
        on();
        return true;
    }
    return false;
}

void PixelRing::off()
{
    brightness_override = 0;
    Serial.println("PixelRing::off: turning off");
}

void PixelRing::on()
{
    brightness_override = 1;
    Serial.println("PixelRing::on: turning on");
}

void PixelRing::incrementWidth(int8_t pixels)
{
    arc_view.incrementArc(pixels);
}

void PixelRing::fullWidth()
{
    arc_view.fullWidth();
}

void PixelRing::shift(int8_t pixels)
{
    arc_view.rotate(pixels);
}

void PixelRing::nextScene()
{
    auto next = [&]()
    {
        return static_cast<SceneMode>(static_cast<std::underlying_type<SceneMode>::type>(last_scene_mode) + 1);
    };

    last_scene_mode = next();
    if (last_scene_mode == SceneMode::Off)
        last_scene_mode = next();

    if (last_scene_mode == SceneMode::None || last_scene_mode == SceneMode::Off)
        last_scene_mode = SceneMode::White;
}

PixelRing::ArcBasedView::ArcBasedView(Adafruit_NeoPixel &strip) :
        strip(strip),
        begin(0),
        end(static_cast<uint8_t>(strip.numPixels() - 1)),
        pixel_iterator(0),
        toggle(0)
{}

void PixelRing::ArcBasedView::process(uint32_t new_color)
{
    this->color = new_color;
    process();
}

void PixelRing::ArcBasedView::process()
{
    uint32_t *color_ptr = &color;
    uint32_t black = Strip::Color(0, 0, 0);

    pixel_iterator = begin;
    do
    {
        strip.setPixelColor(pixel_iterator, *color_ptr);
        strip.show();
        if (pixel_iterator == end)
            color_ptr = &black;
    } while (++pixel_iterator != begin);
}

void PixelRing::ArcBasedView::rotate(int8_t pixels)
{
    begin += pixels;
    end += pixels;
}

void PixelRing::ArcBasedView::incrementArc(int8_t pixels)
{
    Serial.print("PixelRing::ArcBasedView::incrementArc: ");
    Serial.println(pixels);

    while (pixels < 0)
    {
        incrementArcByOne(false);
        ++pixels;
    }

    while (pixels > 0)
    {
        incrementArcByOne(true);
        --pixels;
    }
}

void PixelRing::ArcBasedView::incrementArcByOne(bool do_increment)
{
    int8_t increment = do_increment ? 1 : -1;

    // disallow underflow
    if (!do_increment && begin == end)
        return;

    uint8_t previous_begin = begin, previous_end = end;

    // do the increment
    if (toggle++ == 0)
    {
        begin -= increment;
    } else
    {
        end += increment;
    }

    // revert on overflow
    if (do_increment && begin == end)
    {
        end = previous_end;
        begin = previous_begin;
    }
}

void PixelRing::ArcBasedView::fullWidth()
{
    begin = 0;
    end = 0;
    --end;
}


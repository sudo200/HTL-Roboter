#include "rgb_led.hpp"

CRGB RGB_LED::leds[RGB_LED::leds_size] = {};

void RGB_LED::init() {
    FastLED.addLeds<SK9822, RGB_LED_DATA, RGB_LED_CLOCK, RGB>(leds, 4);
    FastLED.clear(true);
}

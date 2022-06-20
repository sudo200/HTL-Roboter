#ifndef  RGB_LED_HPP
#define  RGB_LED_HPP

#include <Arduino.h>

#include <el_robot.h>

#include <FastLED.h>

class RGB_LED {
private:
    RGB_LED() = default;

    RGB_LED(RGB_LED &) = default;

    static const uint8_t leds_size = 4;
    static CRGB leds[leds_size];

public:
    static void init();

    static void set(uint8_t r, uint8_t g, uint8_t b);
};

inline void RGB_LED::set(uint8_t r, uint8_t g, uint8_t b) {
    for (CRGB &led: leds) {
        led.r = r;
        led.g = b;
        led.b = g;
    }
    FastLED.show();
}

#endif// RGB_LED_HPP

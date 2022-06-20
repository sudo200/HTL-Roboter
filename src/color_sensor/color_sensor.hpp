#ifndef  COLOR_SENSOR_HPP
#define  COLOR_SENSOR_HPP

#include <Arduino.h>
#include <stdint.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include <el_robot.h>

class ColorSensor {
public:
    typedef struct {
        float red;
        float green;
        float blue;
    } RGB;

private:
    typedef struct {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t c;
    } RGB_RAW;


    ColorSensor() = default;

    ColorSensor(ColorSensor &) = default;

    static Adafruit_TCS34725 sensor;
    static volatile RGB rgb;
    static volatile RGB_RAW rgb_raw;

    static void interrupt();

public:
    static void init();

    /**
     * Get a measurement from the sensor
     */
    static void measure();

    /**
     * Get the last measured value
     */
    static RGB getRGB();

    /**
     * Get the last measured color temperature
     */
    static uint16_t getColorTemp();

    /**
     * Get the last measured lux value
     */
    static uint16_t getLux();

    /**
     * Set the state of the ColorSensor LED
     */
    static void setLED(bool state);

    /**
     * Returns true, if the given RGB-value is in tolerance
     */
    static bool isNear(RGB rgb, uint8_t tolerance);

    /**
     * Exception, which is thrown in init() when no sensor could be detected
     */
    class sensornotfound : public std::exception {
        virtual const char *what() const noexcept {
            return "No TCS34725 sensor was found!";
        };
    };
};

inline ColorSensor::RGB ColorSensor::getRGB() {
    return *(RGB *) &rgb;
}

inline uint16_t ColorSensor::getColorTemp() {
    return sensor.calculateColorTemperature(rgb_raw.r, rgb_raw.g, rgb_raw.b);
}

inline uint16_t ColorSensor::getLux() {
    return sensor.calculateLux(rgb_raw.r, rgb_raw.g, rgb_raw.b);
}

inline void ColorSensor::setLED(bool state) {
    digitalWrite(CS_LED, state ? HIGH : LOW);
}

#endif// COLOR_SENSOR_HPP

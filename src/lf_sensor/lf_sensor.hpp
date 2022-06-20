#ifndef  LF_SENSOR_HPP
#define  LF_SENSOR_HPP

#include <Arduino.h>
#include <stdint.h>

#include <el_robot.h>


class LF_Sensor {
private:
    LF_Sensor() = default;

    LF_Sensor(LF_Sensor &) = default;

protected:
    static volatile uint16_t threshold;

public:
    typedef enum {
        right = 0,
        left = 1
    } sensor;

    /**
     * Init sensor.
     */
    static void init();

    /**
     * Set which sensor to measure from.
     */
    static void setSensor(sensor s);

    /**
     * Set Threshold, under which getDigital() returns true.
     */
    static void setThreshold(uint16_t t);

    /**
     * Get raw sensor output.
     */
    static uint16_t getAnalog();

    /**
     * Returns true, if value from sensor is lower than threshold.
     */
    static bool getDigital();
};

inline void LF_Sensor::setSensor(sensor s) {
    digitalWrite(LF_SELECT, s);
}

inline void LF_Sensor::setThreshold(uint16_t t) {
    threshold = t;
}

inline uint16_t LF_Sensor::getAnalog() {
    return analogRead(LF_SENSOR_SIGNAL);
}

inline bool LF_Sensor::getDigital() {
    return getAnalog() < threshold;
}

#endif// LF_SENSOR_HPP

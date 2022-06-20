#ifndef  BAT_UNDERVOLT_DETECT_HPP
#define  BAT_UNDERVOLT_DETECT_HPP

#include <Arduino.h>
#include <stdint.h>

#include <el_robot.h>

#define BAT_UNDERVOLT_THRESEHOLD    2535

class Undervolt_Detection {
private:
    Undervolt_Detection() = default;

    Undervolt_Detection(Undervolt_Detection &) = default;

protected:
    static uint32_t accumulator;
    static uint8_t i;
    static bool undervolt;

public:
    static void init();

    static void check();

    static bool isUndervolt();

    static float getVoltage();
};

inline bool Undervolt_Detection::isUndervolt() {
    return undervolt;
}

inline float Undervolt_Detection::getVoltage() {
    return analogRead(BAT) * .0011843711843711844F;
}

#endif// BAT_UNDERVOLT_DETECT_HPP

#include "lf_sensor.hpp"

volatile uint16_t LF_Sensor::threshold = 0;

void LF_Sensor::init() {
    pinMode(LF_SELECT, OUTPUT);
    pinMode(LF_SENSOR_SIGNAL, INPUT);
}

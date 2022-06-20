#include "bat_undervolt_detect.hpp"

uint32_t Undervolt_Detection::accumulator = 0;
uint8_t Undervolt_Detection::i = 0;
bool Undervolt_Detection::undervolt = false;

void Undervolt_Detection::init() {
    pinMode(BAT, INPUT);
}

void Undervolt_Detection::check() {
    accumulator += analogRead(BAT);
    undervolt = false;
    if (i++ < 200)
        return;

    undervolt = ((accumulator / 200) < BAT_UNDERVOLT_THRESEHOLD);
}

#include "us_sensor.hpp"

volatile uint64_t Ultrasonic::starttime = 0;
volatile uint64_t Ultrasonic::stoptime = 0;

void Ultrasonic::interrupt_start() {
    starttime = esp_timer_get_time();
    attachInterrupt(digitalPinToInterrupt(US_ECHO), interrupt_stop, FALLING);
}

void Ultrasonic::interrupt_stop() {
    stoptime = esp_timer_get_time();
    attachInterrupt(digitalPinToInterrupt(US_ECHO), interrupt_start, RISING);
}

void Ultrasonic::init() {
    pinMode(US_TRIGGER, OUTPUT);
    digitalWrite(US_TRIGGER, LOW);

    pinMode(US_ECHO, INPUT);

    attachInterrupt(digitalPinToInterrupt(US_ECHO), interrupt_start, RISING);
}

void Ultrasonic::trigger() {
    digitalWrite(US_TRIGGER, HIGH);
    delayMicroseconds(TRIGGER_LEN);
    digitalWrite(US_TRIGGER, LOW);

}

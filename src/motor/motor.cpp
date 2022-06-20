#include "motor.hpp"

#define STOP    0

volatile uint64_t 
        Motor::motl_deg = 0,
        Motor::motr_deg = 0;

TaskHandle_t Motor::task0 = nullptr;

const uint8_t Motor::motor_pins[5] = {
        MOTL_SPEED, // Left first
        MOTL_DIR,

        MOTR_DIR, // Right second
        MOTR_SPEED,

        0};

void Motor::motor_init() {
    pinMode(MOTL_SPEED, OUTPUT);
    pinMode(MOTL_DIR, OUTPUT);
    pinMode(MOTR_SPEED, OUTPUT);
    pinMode(MOTR_DIR, OUTPUT);

    digitalWrite(MOTL_DIR, LOW);
    digitalWrite(MOTL_SPEED, LOW);
    digitalWrite(MOTR_DIR, LOW);
    digitalWrite(MOTR_SPEED, LOW);

    pinMode(MOTL_FEEDBACK, INPUT);
    pinMode(MOTR_FEEDBACK, INPUT);

    attachInterrupt(digitalPinToInterrupt(MOTL_FEEDBACK), motl_interrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MOTR_FEEDBACK), motr_interrupt, CHANGE);

    for (uint8_t i = 0; i < 4; i++) {
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(motor_pins[i], i);
        ledcWrite(i, 0);
    }
}

void Motor::motor_left(direction dir, uint16_t speed) {
    if (speed == 0) { // Stop
        ledcWrite(0, STOP);
        ledcWrite(1, STOP);
        return;
    }

    if (dir == CV) {
        ledcWrite(1, 0);
        ledcWrite(0, speed);
    } else { // dir == CCV
        ledcWrite(0, 0);
        ledcWrite(1, speed);
    }
}

void Motor::motor_right(direction dir, uint16_t speed) {
    if (speed == 0) { // Stop
        ledcWrite(2, STOP);
        ledcWrite(3, STOP);
        return;
    }

    if (dir == CCV) {
        ledcWrite(3, 0);
        ledcWrite(2, speed);
    } else { // dir == CV
        ledcWrite(2, 0);
        ledcWrite(3, speed);
    }
}

void Motor::motl_interrupt() {
    motl_deg += DEG_PER_PULSE;
}

void Motor::motr_interrupt() {
    motr_deg += DEG_PER_PULSE;
}

#ifndef MOTOR_HPP
#define MOTOR_HPP

#include <Arduino.h>
#include <cstdint>

#include <el_robot.h>

#define PWM_FREQUENCY 16000 // Hz
#define PWM_RESOLUTION 10 // bit

#define MAX_VALUE      ((1 << (PWM_RESOLUTION + 1)) - 1)

#define DEG_PER_PULSE   9
#define WHEEL_RADIUS    .0325 // m

class Motor {
public:
    typedef enum {
        CV = 0, // Clockvise
        CCV = 1 // Counter-Clockvise
    } direction;
private:
    Motor() = default;
    Motor(Motor &) = default;
    Motor &operator=(Motor &) = default;


    static const uint8_t motor_pins[5];

    volatile static uint64_t motl_deg, motr_deg;

    static TaskHandle_t task0;

    static void motl_interrupt();
    static void motr_interrupt();

public:
    static void motor_init();

    static void motor_left(direction dir, uint16_t speed);

    static void motor_right(direction dir, uint16_t speed);
};

#endif // MOTOR_HPP

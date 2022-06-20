#ifndef  US_SENSOR_HPP
#define  US_SENSOR_HPP

#include <Arduino.h>
#include <stdint.h>

#include <el_robot.h>

// Pulse time for trigger
#define TRIGGER_LEN 10
// Speed of ultrasound [m/s]
#define US_SPEED    340.278

class Ultrasonic {
private:
    Ultrasonic() = default;

    Ultrasonic(Ultrasonic &) = default;

    volatile static uint64_t starttime, stoptime;

    static void interrupt_start();

    static void interrupt_stop();

public:
    /**
     * Initializes the sensor
     */
    static void init();

    /**
     * Triggers a signal to be sent.
     * 
     * Best practice: Call in main loop.
     */
    static void trigger();

    /**
     * Returns the last measured distance in millimeters
     */
    static uint16_t getDistance();
};

inline uint16_t Ultrasonic::getDistance() {
    return (uint16_t) ((US_SPEED * ((stoptime - starttime) / (long double) 2000000)) * 1000);
}

#endif// US_SENSOR_HPP

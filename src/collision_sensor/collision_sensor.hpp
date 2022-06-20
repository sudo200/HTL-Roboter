#ifndef  COLLISION_SENSOR_HPP
#define  COLLISION_SENSOR_HPP

#include <Arduino.h>

#include <el_robot.h>


class CollisionSensor {
private:
    CollisionSensor() = default;
    CollisionSensor(CollisionSensor&) = default;
    CollisionSensor& operator=(CollisionSensor&) = default;

public:
    static void init() {
        pinMode(COL_LEFT, INPUT_PULLUP);
        pinMode(COL_RIGHT, INPUT_PULLUP);
    }

    static bool collisionLeft() {
        return !digitalRead(COL_LEFT);
    }

    static bool collisionRight() {
        return !digitalRead(COL_RIGHT);
    }
};

#endif// COLLISION_SENSOR_HPP

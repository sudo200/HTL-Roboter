/* Fabian Gratzer | 25.4.2022
 * El-Robot
 */

#include "settings.h"

#include <Arduino.h>
#include <el_robot.h>
#include <cstdint>

#include "motor/motor.hpp"
#include "bat_undervolt_detect/bat_undervolt_detect.hpp"
#include "lf_sensor/lf_sensor.hpp"
#include "us_sensor/us_sensor.hpp"
#include "collision_sensor/collision_sensor.hpp"
#include "color_sensor/color_sensor.hpp"
#include "rgb_led/rgb_led.hpp"
#include "lua_api/lua_api.hpp"

#include <BluetoothSerial.h>

#include <lua.hpp>

#include <SPIFFS.h>

#define DELAY 50          // ms
#define SLEEPTIME 4000000 // µs

volatile uint8_t undervolt_i = 0;
volatile bool isInUndervolt = false;

TaskHandle_t task1;
lua_State *L;
BluetoothSerial SerialBT;

[[noreturn]] void loop0(void *);

volatile void delay_loop() {
    if (isInUndervolt) {
        Undervolt_Detection::check();
        if (undervolt_i++ > 200 && Undervolt_Detection::isUndervolt()) { // Undervoltage confirmed
            undervolt_i = 0;
            digitalWrite(ESP_LED, LOW);
            esp_deep_sleep(SLEEPTIME);
        }
    } else {
        Undervolt_Detection::check();
        isInUndervolt = Undervolt_Detection::isUndervolt();
    }
}

void setup() {
    Serial.begin(9600);
    Motor::motor_init();
    RGB_LED::init();
    Undervolt_Detection::init();

#ifdef CHARGING
    esp_deep_sleep(UINT64_MAX);
#endif // CHARGING

    SerialBT.begin("Geitzi_Gratzer");
    LF_Sensor::init();
    Ultrasonic::init();
    CollisionSensor::init();
    ColorSensor::init();

    // LED
    pinMode(ESP_LED, OUTPUT);
    digitalWrite(ESP_LED, HIGH);
    delay_loop();

    // Tasks
    xTaskCreateUniversal(loop0, "loop0", 0xFFF, nullptr, 2, &task1, 0);

    SPIFFS.begin(true);

    // Lua
    L = luaL_newstate();

    luaopen_base(L);// kindof not platform independent...
    luaopen_coroutine(L);
    lua_setglobal(L, "coroutine");
    luaopen_math(L);
    lua_setglobal(L, "math");
    luaopen_package(L);
    lua_setglobal(L, "package");
    luaopen_string(L);
    lua_setglobal(L, "string");
    luaopen_table(L);
    lua_setglobal(L, "table");

    luaopen_battery(L);
    luaopen_collisionsensor(L);
    luaopen_colorsensor(L);
    luaopen_lf_sensor(L);
    luaopen_motor(L);
    luaopen_us_sensor(L);
    luaopen_esp(L);

    const luaL_Reg bluetooth[] = {
            {"has_client", [](lua_State *L) {
                lua_pushboolean(L, SerialBT.hasClient());
                return 1;
            }},
            {"available", [](lua_State *L) {
                lua_pushboolean(L, !!SerialBT.available());
                return 1;
            }},
    };
    luaL_newlib(L, bluetooth);
    lua_setglobal(L, "bluetooth");

    lua_pushcfunction(L, [](lua_State *L) {
        int n = lua_gettop(L);
        for (int i = 1; i <= n; i++) {
            if(i > 1)
                SerialBT.print('\t');
            SerialBT.print(luaL_tolstring(L, i, nullptr));
            lua_pop(L, 1);
        }
        SerialBT.println();
        return 0;
    });
    lua_setglobal(L, "print");

    lua_pushcfunction(L, [](lua_State *L) {
        lua_pushstring(L, SerialBT.readString().c_str());
        return 1;
    });
    lua_setglobal(L, "scan");
}

namespace RGB_states {
    typedef enum {
        INIT,
        GREEN_ON,
        RED_OFF,
        BLUE_ON,
        GREEN_OFF,
        RED_ON,
        BLUE_OFF,
    } state;
}
using RGB_states::state;

[[noreturn]] void loop0(void *p) // Core 0
{
    while (true) {
        EVERY_N_MILLIS(5 /*ms*/) {// RGB State machine
            static state s = RGB_states::INIT;
            static uint8_t r = 0, g = 0, b = 0;

            switch (s) // State Machine for RRRGGGBBB!!! 🌈
            {
                case RGB_states::INIT: {
                    if (++r >= 0xFF)
                        s = RGB_states::GREEN_ON;
                }
                    break;
                case RGB_states::GREEN_ON: {
                    if (++g >= 0xFF)
                        s = RGB_states::RED_OFF;
                }
                    break;
                case RGB_states::RED_OFF: {
                    if (--r <= 0)
                        s = RGB_states::BLUE_ON;
                }
                    break;
                case RGB_states::BLUE_ON: {
                    if (++b >= 0xFF)
                        s = RGB_states::GREEN_OFF;
                }
                    break;
                case RGB_states::GREEN_OFF: {
                    if (--g <= 0)
                        s = RGB_states::RED_ON;
                }
                    break;
                case RGB_states::RED_ON: {
                    if (++r >= 0xFF)
                        s = RGB_states::BLUE_OFF;
                }
                    break;
                case RGB_states::BLUE_OFF: {
                    if (--b <= 0)
                        s = RGB_states::GREEN_ON;
                }
                    break;
            }

            RGB_LED::set(r, g, b);
        }

        EVERY_N_MILLIS(DELAY) {// Battery watch & color sensor
            ColorSensor::measure();
            delay_loop();
        }
    }
}

void loop /*1*/ () // Core 1
{
    File file = SPIFFS.open("/main.lua");
    const char *code = file.readString().c_str();
    file.close();

    luaL_loadstring(L,code);
    if(lua_pcall(L, 0, LUA_MULTRET, 0) != LUA_OK) {
        SerialBT.printf("Error: %s\n", lua_tostring(L, -1));
    }
}

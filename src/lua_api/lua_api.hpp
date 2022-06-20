#ifndef GRATZER_EL_ROBOT_LUA_API_HPP
#define GRATZER_EL_ROBOT_LUA_API_HPP

#include <cmath>

#include <lua.hpp>

#include "../bat_undervolt_detect/bat_undervolt_detect.hpp"
#include "../collision_sensor/collision_sensor.hpp"
#include "../color_sensor/color_sensor.hpp"
#include "../lf_sensor/lf_sensor.hpp"
#include "../motor/motor.hpp"
#include "../us_sensor/us_sensor.hpp"

inline LUAMOD_API int luaopen_battery(lua_State *L) {
    const luaL_Reg battery[] = {
            {"voltage", [](lua_State *L) {
                lua_pushnumber(L, Undervolt_Detection::getVoltage());
                return 1;
            }}
    };
    luaL_newlib(L, battery);
    lua_setglobal(L, "battery");
    return 1;
}

inline LUAMOD_API int luaopen_collisionsensor(lua_State *L) {
    const luaL_Reg collisionsensor[] = {
            {"left", [](lua_State *L) {
                lua_pushboolean(L, CollisionSensor::collisionLeft());
                return 1;
            }},
            {"right", [](lua_State *L) {
                lua_pushboolean(L, CollisionSensor::collisionRight());
                return 1;
            }}
    };
    luaL_newlib(L, collisionsensor);
    lua_setglobal(L, "collisionsensor");
    return 1;
};

inline LUAMOD_API int luaopen_colorsensor(lua_State *L) {
    const luaL_Reg colorsensor[] = {
            {"get_rgb", [](lua_State *L) {
                const ColorSensor::RGB rgb = ColorSensor::getRGB();
                lua_pushnumber(L, rgb.red);
                lua_pushnumber(L, rgb.green);
                lua_pushnumber(L, rgb.blue);
                return 3;
            }},
            {"get_colortemp", [](lua_State *L) {
                lua_pushinteger(L, ColorSensor::getColorTemp());
                return 1;
            }},
            {"get_lux", [](lua_State *L) {
                lua_pushinteger(L, ColorSensor::getLux());
                return 1;
            }},
            {"led", [](lua_State *L) {
                if(!lua_isboolean(L, 1))
                    luaL_error(L, "bad argument #1 to 'led' (boolean expected, got %s)", lua_type(L, 1));
                ColorSensor::setLED(lua_toboolean(L, 1));
                return 0;
            }},
            {"is_near", [](lua_State *L) {
                ColorSensor::RGB rgb = {
                        static_cast<float>(luaL_checknumber(L, 1)),
                        static_cast<float>(luaL_checknumber(L, 2)),
                        static_cast<float>(luaL_checknumber(L, 3))
                };
                lua_Integer tolerance = luaL_checkinteger(L, 4);
                if(tolerance < 0)
                    luaL_error(L, "bad argument #4 to 'is_near' (number cannot be smaller than zero)");
                lua_pushboolean(L, ColorSensor::isNear(rgb, tolerance));
                return 1;
            }},
    };
    luaL_newlib(L, colorsensor);
    lua_setglobal(L, "colorsensor");
    return 1;
};

inline LUAMOD_API int luaopen_lf_sensor(lua_State *L) {
    const luaL_Reg lf_sensor[] = {
            {"set_sensor", [](lua_State *L) {
                if(!lua_isboolean(L, 1))
                    luaL_error(L, "bad argument #1 to 'set_sensor' (boolean expected, got %s)", lua_type(L, 1));
                LF_Sensor::setSensor(lua_toboolean(L, 1) ? LF_Sensor::left : LF_Sensor::right);
                return 0;
            }},
            {"set_threshold", [](lua_State *L) {
                lua_Integer threshold = luaL_checkinteger(L, 1);
                if(threshold < 0)
                    luaL_error(L, "bad argument #1 to 'set_threshold' (number cannot be smaller than zero)");
                LF_Sensor::setThreshold(threshold);
                return 0;
            }},
            {"get_analog", [](lua_State *L) {
                lua_pushinteger(L, LF_Sensor::getAnalog());
                return 1;
            }},
            {"get_digital", [](lua_State *L) {
                lua_pushboolean(L, LF_Sensor::getDigital());
                return 1;
            }},
    };
    luaL_newlib(L, lf_sensor);
    lua_setglobal(L, "lf_sensor");
    return 1;
};

inline LUAMOD_API int luaopen_motor(lua_State *L) {
    const luaL_Reg motor[] = {
            {"left", [](lua_State *L) {
                if(!lua_isboolean(L, 1))
                    luaL_error(L, "bad argument #1 to 'left' (boolean expected, got %s)", lua_type(L, 1));
                Motor::motor_left(
                        lua_toboolean(L, 1) ? Motor::CCV : Motor::CV,
                        abs(luaL_checkinteger(L, 2))
                        );
                return 0;
            }},
            {"right", [](lua_State *L) {
                if(!lua_isboolean(L, 1))
                    luaL_error(L, "bad argument #1 to 'right' (boolean expected, got %s)", lua_type(L, 1));
                Motor::motor_right(
                        lua_toboolean(L, 1) ? Motor::CCV : Motor::CV,
                        abs(luaL_checkinteger(L, 2))
                );
                return 0;
            }},
    };
    luaL_newlib(L, motor);
    lua_setglobal(L, "motor");
    return 1;
};

inline LUAMOD_API int luaopen_us_sensor(lua_State *L) {
    const luaL_Reg us_sensor[] = {
            {"trigger", [](lua_State *L) {
                Ultrasonic::trigger();
                return 0;
            }},
            {"distance", [](lua_State *L) {
                lua_pushinteger(L, Ultrasonic::getDistance());
                return 1;
            }},
    };
    luaL_newlib(L, us_sensor);
    lua_setglobal(L, "us_sensor");
    return 1;
};

inline LUAMOD_API int luaopen_esp(lua_State *L) {
    const luaL_Reg esp[] = {
            {"millis", [](lua_State *L) {
                lua_pushinteger(L, static_cast<lua_Integer>(millis()));
                return 1;
            }},
            {"micros", [](lua_State *L) {
                lua_pushinteger(L, static_cast<lua_Integer>(micros()));
                return 1;
            }},
            {"delay", [](lua_State *L) {
                delay(abs(luaL_checkinteger(L, 1)));
                return 0;
            }},
            {"delayMicroseconds", [](lua_State *L) {
                delayMicroseconds(abs(luaL_checkinteger(L, 1)));
                return 0;
            }},
    };
    luaL_newlib(L, esp);
    lua_setglobal(L, "esp");
    return 1;
};

#endif //GRATZER_EL_ROBOT_LUA_API_HPP

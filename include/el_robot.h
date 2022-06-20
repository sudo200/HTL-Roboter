/**
 * Contains Pin-Definitions
 * and utilities
 * for the EL-Robot
 */

typedef void (*callback_t)();

// Right Motor
#define MOTR_SPEED  2
#define MOTR_DIR    15

// Left Motor
#define MOTL_SPEED  32
#define MOTL_DIR    33

// Battery | through voltage divider
#define BAT         39

// Onboard LED of the LoLin32
#define ESP_LED     5

// Linefollower Sensors
#define LF_SENSOR_SIGNAL    36
#define LF_SELECT           12

// Ultrasonic sensor
#define US_TRIGGER  25
#define US_ECHO     26

// Collision sensors
#define COL_LEFT    34
#define COL_RIGHT   35

// Color sensor
#define CS_INTERRUPT    4
#define CS_LED          13

// Motor feedback
#define MOTL_FEEDBACK   27
#define MOTR_FEEDBACK   14

// RGB LEDs
#define RGB_LED_CLOCK   18
#define RGB_LED_DATA    23

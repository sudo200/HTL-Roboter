#include "color_sensor.hpp"

Adafruit_TCS34725 ColorSensor::sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
volatile ColorSensor::RGB ColorSensor::rgb = {0, 0, 0};
volatile ColorSensor::RGB_RAW ColorSensor::rgb_raw = {0, 0, 0, 0};

void ColorSensor::interrupt() {

}

void ColorSensor::init() {
    pinMode(CS_INTERRUPT, INPUT_PULLUP);
    pinMode(CS_LED, OUTPUT);

    if (!sensor.begin())
        throw sensornotfound();

    digitalWrite(CS_LED, LOW);
    //attachInterrupt(digitalPinToInterrupt(CS_INTERRUPT), interrupt, FALLING);

    sensor.setInterrupt(false);
    sensor.clearInterrupt();
}

void ColorSensor::measure() {
    sensor.getRGB((float *) &rgb.red, (float *) &rgb.green, (float *) &rgb.blue);
    sensor.getRawData((uint16_t *) &rgb_raw.r, (uint16_t *) &rgb_raw.g, (uint16_t *) &rgb_raw.b, (uint16_t *) &rgb_raw.c);
}

bool ColorSensor::isNear(RGB rgb, uint8_t tolerance) {
    if(ColorSensor::rgb.red > (rgb.red + tolerance) || ColorSensor::rgb.red < (rgb.red - tolerance))
        return false;
    if(ColorSensor::rgb.green > (rgb.green + tolerance) || ColorSensor::rgb.green < (rgb.green - tolerance))
        return false;
    if(ColorSensor::rgb.blue > (rgb.blue + tolerance) || ColorSensor::rgb.blue < (rgb.blue - tolerance))
        return false;
    return true;
}

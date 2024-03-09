#ifndef DISTANCE_SENSOR
#define DISTANCE_SENSOR
#include "Arduino.h"
#include "Adafruit_VL53L0X.h"
extern Adafruit_VL53L0X lox;
void setupVL53L0X();
uint16_t  readVL53L0X();
#endif
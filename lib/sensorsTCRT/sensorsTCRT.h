#ifndef SENSORTCRT_H
#define SENSORTCRT_H
#include <Arduino.h>

#ifndef DEBUG_SERIAL_PRINT 
#define DEBUG_SERIAL_PRINT 0
#endif

#if DEBUG_SERIAL_PRINT
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

#define SENSOR_COUNT 8

#define WHITE_LINE 1
#define BLACK_LINE 0
/*______________________________________SENSORS______________________________________________________*/

extern const  byte posSensor[];
extern bool s[];
extern int dataSensor;
extern float lastOnLineSomme;
extern float somme;
extern int cnt;
extern bool onLine;
extern float lastOnLineSomme;


void setupSensors();
void readSensors();
void printSensors();

#endif
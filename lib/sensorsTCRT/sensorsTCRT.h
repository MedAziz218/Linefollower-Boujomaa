#ifndef SENSORTCRT_H
#define SENSORTCRT_H
#include <Arduino.h>


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
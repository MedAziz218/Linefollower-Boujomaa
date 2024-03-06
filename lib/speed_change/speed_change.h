#ifndef SPEED_CHANGE_H
#define SPEED_CHANGE_H
#include "Arduino.h"
#include "arduino-timer.h"
#include "pid_classes.h"
#include "motor_encoder.h"
/*===========================================
    Define Speed Change Data and Timer
===========================================*/
extern Timer<1, micros> speedChangeTimer;
bool isSpeedChangeRunning();
void reset_phase();
int get_phase();
void startSpeedChange(_base_pid *_pid, int _startpeed, int _endspeed, int _startdist, int _durationdist);



#endif
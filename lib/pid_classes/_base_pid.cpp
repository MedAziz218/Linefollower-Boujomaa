
#include "pid_classes.h"

void _base_pid::_compute_pid_and_setmotors(int error)
{
    double deltaTime = (micros() - lastProcess) / 1000000.0;
    P = error * (double)kp;
    D = (error - lastError) * (double)kd / deltaTime;

    double rateError = error - lastError;
    lastError = error;
    lastProcess = micros();

    double mv = P + D;
    int moveVal = (int)(P + D);
    int moveLeft = basespeed - moveVal;
    int moveRight = basespeed + moveVal;

    moveLeft = constrain(moveLeft, minspeed, maxspeed);
    moveRight = constrain(moveRight, minspeed, maxspeed);
    setMotors(moveLeft, moveRight);
    
    lastMoveLeft = moveLeft;
    lastMoveRight = moveRight;
}
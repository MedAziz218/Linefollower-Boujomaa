/**
 * @file pid_classes.h
 * @brief Library providing PID control classes for robot line-following.
 *
 * This library offers four PID control classes tailored for line-following robots, each with distinct behaviors and error calculation methods:
 * - Corrected classes remember the last seen line position and adjust the robot's trajectory to return to it, useful for maintaining accurate path tracking.
 * - Uncorrected classes maintain forward movement without correction when the line isn't detected, suitable for scenarios with intermittent or dotted lines.
 *
 * Control Criteria:
 * 1. Corrected/Uncorrected:
 *    - Corrected: Adjusts trajectory based on the last line position when line isn't detected.
 *    - Uncorrected: Continues forward movement without correction when line isn't detected.

 * 2. Error Calculation Methods:
 *    - Sum: Calculates error by summing sensor weights, where weights vary to prioritize central sensor readings.
 *    - Switchcase: Detects clear lines using adjacent sensors, ideal for scenarios with multiple lines or distorted regions.
 *
 * Usage:
 * - Select a PID control class based on your robot's requirements.
 * - Ensure to call `readsensors` before invoking `.Compute`.
 * - Periodically call `.Compute` to update control actions.
 * 
 * Dependencies:
 * - Requires `sensorsTCRT.h` for sensor data.
 * - Depends on `motor_encoder.h` for motor control.
 * 
 */



#ifndef PID_CLASSES_H
#define PID_CLASSES_H

#include <Arduino.h>
#include "sensorsTCRT.h"
#include "motor_encoder.h"

#ifndef SENSOR_COUNT
#define SENSOR_COUNT 8
#endif

extern void setMotors(int speed1, int speed2); // Forward declaration of setMotors function
extern bool s[];
extern int dataSensor;
extern float lastOnLineSomme;
extern int lastOnLineDataSensor;

/*______________________________________BASE_PID_CLASSES(VIRTUAL)______________________________________________________*/
/**
 * @brief Abstract base class for PID control.
 * 
 * Provides common functionality for PID control.
 */
class _base_pid
{
public:
    float kp;
    float kd;
    int basespeed, minspeed, maxspeed;
    int lastMoveLeft, lastMoveRight;
    int lastOnLineError;
    int lastError;
    // clang-format off
    void setKp_kd_min_max(int p_kp, int p_kd, int p_minspeed, int p_maxspeed){kp = p_kp; kd = p_kd;minspeed = p_minspeed;maxspeed = p_maxspeed;}
    void setKp(float p_kp){kp = p_kp;}
    void setKd(float p_kd){kd = p_kd;}
    void setBaseSpeed(int p_basespeed){basespeed = p_basespeed;}
    void setMaxSpeed(int p_maxspeed){maxspeed = p_maxspeed;}
    void setMinSpeed(int p_minspeed){minspeed = p_minspeed;}
    // clang-format on
    virtual void Compute() = 0;

protected:
    double  P, D;
    unsigned long lastProcess;
    void _compute_pid_and_setmotors(int error);
};
/**
 * @brief Abstract base class for PID control using sum method.
 * 
 * Provides common functionality for PID control using sum method.
 */
class _base_pid_sum : public _base_pid
{
public:
    virtual void Compute() = 0;

protected:
    // clang-format off
    int weights[8] =  {-100, -50, -25, -10, 10, 25, 50, 100};
    void setWeights(int vals[8]){ for (int i = 0; i < 8; i++)weights[i] = vals[i]; };
    int _calculate_error_with_sum_method();
    // clang-format on
};
/**
 * @brief Abstract base class for PID control using switch-case method.
 * 
 * Provides common functionality for PID control using switch-case method.
 */
class _base_pid_switchcase : public _base_pid
{
public:
    virtual void Compute() = 0;

protected:
    int _calculate_error_with_switchcase_method(int shift = 0);
};

/*______________________________________PID_CLASSES______________________________________________________*/
/**
 * @brief PID control class using sum method without correction.
 */
class PID_Sum_Uncorrected_cls : public _base_pid_sum
{
public:
    void Compute() override;
};
/**
 * @brief PID control class using switch-case method without correction.
 */
class PID_SwitchCase_Uncorrected_cls : public _base_pid_switchcase
{
public:
    void Compute() override;
};
/**
 * @brief PID control class using sum method with correction.
 */
class PID_Sum_Corrected_cls : public _base_pid_sum
{
public:
    int correctionValue = 100;
    void setCorrectionValue(int p_val) { correctionValue = p_val; }
    void Compute() override;
};
/**
 * @brief PID control class using switch-case method with correction.
 */
class PID_SwitchCase_Corrected_cls : public _base_pid_switchcase
{
public:
    int correctionValue = 6;
    void setCorrectionValue(int p_val) { correctionValue = p_val; }
    void Compute() override;
};
#endif

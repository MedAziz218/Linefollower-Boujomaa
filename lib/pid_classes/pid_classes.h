#ifndef PID_CLASSES_H
#define PID_CLASSES_H

#ifndef SENSOR_COUNT
#define SENSOR_COUNT 8
#endif

#include <Arduino.h>
extern void setMotors(int speed1, int speed2); // Forward declaration of setMotors function
extern bool s[];
extern int dataSensor;
extern float lastOnLineSomme;
extern int lastOnLineDataSensor;

/*______________________________________BASE_PID_CLASSES(VIRTUAL)______________________________________________________*/

class _base_pid
{
public:
    int kp;
    int kd;
    int basespeed, minspeed, maxspeed;
    int lastOnLineError;
    int lastMoveLeft, lastMoveRight;
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
    int  P, D;
    unsigned long lastProcess;
    void _compute_pid_and_setmotors(int error);
};

class _base_pid_sum : public _base_pid
{
public:
    virtual void Compute() = 0;

protected:
    // clang-format off
    int values[8] =  {-100, -50, -25, -10, 10, 25, 50, 100};
    void setValues(int vals[8]){ for (int i = 0; i < 8; i++)values[i] = vals[i]; };
    int _calculate_error_with_sum_method();
    // clang-format on
};
class _base_pid_switchcase : public _base_pid
{
public:
    virtual void Compute() = 0;

protected:
    int _calculate_error_with_switchcase_method(int shift = 0);
};

/*______________________________________PID_CLASSES______________________________________________________*/

class PID_Sum_Uncorrected_cls : public _base_pid_sum
{
public:
    void Compute() override;
};
class PID_SwitchCase_Uncorrected_cls : public _base_pid_switchcase
{
public:
    void Compute() override;
};

class PID_Sum_Corrected_cls : public _base_pid_sum
{
public:
    int correctionValue = 100;
    void setCorrectionValue(int p_val) { correctionValue = p_val; }
    void Compute() override;
};
class PID_SwitchCase_Corrected_cls : public _base_pid_switchcase
{
public:
    int correctionValue = 6;
    void setCorrectionValue(int p_val) { correctionValue = p_val; }
    void Compute() override;
};
#endif

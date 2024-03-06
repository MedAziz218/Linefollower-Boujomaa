#ifndef MOTORENCODERCONTROL_H
#define MOTORENCODERCONTROL_H
#include <Arduino.h>

// pwm config
#define PWM_Res 8
#define PWM_Freq 100000

// left motor pins
#define PWM_CHANNEL_L 0
#define PIN_PWM_L 33
#define PIN_IN1_L 26
#define PIN_IN2_L 25
#define PIN_encR_A 39 // white
#define PIN_encR_B 36 // yellow

// right motor pins
#define PWM_CHANNEL_R 1
#define PIN_PWM_R 13
#define PIN_IN1_R 27
#define PIN_IN2_R 14
#define PIN_encL_A 35 // white
#define PIN_encL_B 34 // yellow


unsigned int get_encL();
unsigned int get_encR();
unsigned int get_encoders();
void reset_encR();
void reset_encL();
void reset_encoders();
void setupEncoders();
void setupMotors();
void setMotors(int LL, int RR);


#endif
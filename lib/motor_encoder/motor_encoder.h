#ifndef MOTORENCODERCONTROL_H
#define MOTORENCODERCONTROL_H

#include <Arduino.h>

/**
 * @file Motorencodercontrol.h
 * @brief Header file for controlling motors and encoders.
 */

/**
 * @defgroup MotorEncoderControl Motor Encoder Control
 * @brief Functions and configurations for controlling motors and encoders.
 */

/**
 * @brief Resolution of PWM signal.
 * 
 * Resolution of the PWM signal in bits.
 * 
 * @ingroup MotorEncoderControl
 */
#define PWM_Res 8

/**
 * @brief Frequency of PWM signal.
 * 
 * Frequency of the PWM signal in Hz.
 * 
 * @ingroup MotorEncoderControl
 */
#define PWM_Freq 100000

/**
 * @brief Minimum delay between encoder ticks.
 * 
 * Minimum delay between encoder ticks in microseconds.
 * 
 * @ingroup MotorEncoderControl
 */
#define ENCODER_TICK_MIN_DELAY 129

/**
 * @brief Left motor pins configuration.
 * 
 * Pins configuration for the left motor.
 * 
 * @ingroup MotorEncoderControl
 */
#define PWM_CHANNEL_L 0
#define PIN_PWM_L 33
#define PIN_IN1_L 26
#define PIN_IN2_L 25
#define PIN_encR_A 39 // white
#define PIN_encR_B 36 // yellow

/**
 * @brief Right motor pins configuration.
 * 
 * Pins configuration for the right motor.
 * 
 * @ingroup MotorEncoderControl
 */
#define PWM_CHANNEL_R 1
#define PIN_PWM_R 13
#define PIN_IN1_R 27
#define PIN_IN2_R 14
#define PIN_encL_A 35 // white
#define PIN_encL_B 34 // yellow

/**
 * @brief Get encoder count for the left motor.
 * 
 * @return unsigned int Encoder count for the left motor.
 * 
 * @ingroup MotorEncoderControl
 */
unsigned int get_encL();

/**
 * @brief Get encoder count for the right motor.
 * 
 * @return unsigned int Encoder count for the right motor.
 * 
 * @ingroup MotorEncoderControl
 */
unsigned int get_encR();

/**
 * @brief Get encoder counts for both motors.
 * 
 * @return unsigned int Encoder counts for both motors.
 * 
 * @ingroup MotorEncoderControl
 */
unsigned int get_encoders();

/**
 * @brief Reset encoder count for the left motor.
 * 
 * @ingroup MotorEncoderControl
 */
void reset_encL();

/**
 * @brief Reset encoder count for the right motor.
 * 
 * @ingroup MotorEncoderControl
 */
void reset_encR();

/**
 * @brief Reset encoder counts for both motors.
 * 
 * @ingroup MotorEncoderControl
 */
void reset_encoders();

/**
 * @brief Setup encoders.
 * 
 * Initialize encoder pins and setup interrupt handling.
 * 
 * @ingroup MotorEncoderControl
 */
void setupEncoders();

/**
 * @brief Setup motors.
 * 
 * Initialize motor pins and PWM configurations.
 * 
 * @ingroup MotorEncoderControl
 */
void setupMotors();

/**
 * @brief Set motor speeds.
 * 
 * Set speeds for both left and right motors.
 * 
 * @param LL Speed for the left motor. Value ranges from -255 (full reverse) to +255 (full forward).
 * @param RR Speed for the right motor. Value ranges from -255 (full reverse) to +255 (full forward).
 * 
 * @ingroup MotorEncoderControl
 */
void setMotors(int LL, int RR);

#endif

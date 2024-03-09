#include "motor_encoder.h"

/*__________________________________________ENCODER_______________________________________________*/
// Variables to store encoder ticks
volatile unsigned int encR_ticks = 0;
volatile unsigned int encL_ticks = 0;
volatile unsigned long encL_lastTickTime = 0;
volatile unsigned long encR_lastTickTime = 0;

// Function to increment right encoder tick count (Do not use directly)
void read_rightEncoder()
{
  if (micros() - encR_lastTickTime > ENCODER_TICK_MIN_DELAY)
  {
    encR_ticks++;
    encR_lastTickTime = micros();
  }
}
// Function to increment left encoder tick count (Do not use directly)
void read_leftEncoder()
{
  if (micros() - encL_lastTickTime > ENCODER_TICK_MIN_DELAY)
  {
    encL_ticks++;
    encL_lastTickTime = micros();
  }
}
// Function to reset right encoder tick count
void reset_encR() { encR_ticks = 0; }
// Function to reset left encoder tick count
void reset_encL() { encL_ticks = 0; }
// Function to get the current right encoder tick count (equivalent to millis())
unsigned int get_encR() { return encR_ticks; }
// Function to get the current left encoder tick count (equivalent to millis())
unsigned int get_encL() { return encL_ticks; }

unsigned int get_encoders() { return encL_ticks + encR_ticks; }
void reset_encoders()
{
  reset_encL();
  reset_encR();
}

void setupEncoders()
{
  // Configure right encoder pins
  pinMode(PIN_encR_A, INPUT_PULLDOWN);
  pinMode(PIN_encR_B, INPUT_PULLDOWN);
  // Configure left encoder pins
  pinMode(PIN_encL_A, INPUT_PULLDOWN);
  pinMode(PIN_encL_B, INPUT_PULLDOWN);
  // Attach interrupts for both encoders on rising edges
  attachInterrupt(digitalPinToInterrupt(PIN_encR_B), read_rightEncoder, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_encL_B), read_leftEncoder, RISING);
  reset_encoders();
}

/*__________________________________________MOTOR_______________________________________________*/
void setupMotors()
{
  // pinMode(hbridgeA,OUTPUT); pinMode(hbridgeB,OUTPUT);
  // pinMode(encoderPinA,INPUT);pinMode(encoderPinB,INPUT);
  pinMode(PIN_PWM_L, OUTPUT);
  ledcSetup(PWM_CHANNEL_L, PWM_Freq, PWM_Res);
  ledcAttachPin(PIN_PWM_L, PWM_CHANNEL_L);

  pinMode(PIN_PWM_R, OUTPUT);
  ledcSetup(PWM_CHANNEL_R, PWM_Freq, PWM_Res);
  ledcAttachPin(PIN_PWM_R, PWM_CHANNEL_R);

  pinMode(PIN_IN1_L, OUTPUT);
  pinMode(PIN_IN2_L, OUTPUT);
  pinMode(PIN_IN1_R, OUTPUT);
  pinMode(PIN_IN2_R, OUTPUT);

  setMotors(0, 0);
}
void setMotors(int LL, int RR)
{
  LL = constrain(LL, -255, 255);
  RR = constrain(RR, -255, 255);

  if (RR > 0)
  {
    ledcWrite(PWM_CHANNEL_R, RR);
    digitalWrite(PIN_IN1_R, HIGH);
    digitalWrite(PIN_IN2_R, LOW);
  }
  else
  {
    ledcWrite(PWM_CHANNEL_R, -RR);
    digitalWrite(PIN_IN1_R, LOW);
    digitalWrite(PIN_IN2_R, HIGH);
  }
  if (LL > 0)
  {
    ledcWrite(PWM_CHANNEL_L, LL);
    digitalWrite(PIN_IN1_L, LOW);
    digitalWrite(PIN_IN2_L, HIGH);
  }
  else
  {
    ledcWrite(PWM_CHANNEL_L, -LL);
    digitalWrite(PIN_IN1_L, HIGH);
    digitalWrite(PIN_IN2_L, LOW);
  }
}
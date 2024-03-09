#define DEBUG_SERIAL_PRINT 1
/*===========================================
    Include Libraries
===========================================*/
#ifndef INC_FREERTOS_H
#include <freertos/FreeRTOS.h>
#endif
#ifndef INC_TASK_H
#include <freertos/task.h>
#endif

#include "Arduino.h"
#include "sensorsTCRT.h"
#include "pid_classes.h"
#include "motor_encoder.h"
#include "speed_change.h"
#include "arduino-timer.h"
#include "my_timer.h"
// #include "VL53L0X_sensor.h"
/*===========================================
   Conditional Compilation for Debugging
===========================================*/
#define ENABLE_DEBUG_LOGGING 1
#if ENABLE_DEBUG_LOGGING
#define DEBUG_LOG(x) Serial.print(x)
#define DEBUG_LOGLN(x) Serial.println(x)
#define DEBUG_LOG_BEGIN(x) Serial.begin(x)
#else
#define DEBUG_LOG(x)
#define DEBUG_LOGLN(x)
#define DEBUG_LOG_BEGIN(x)
#endif
/*===========================================
             Define LED pins
===========================================*/
#define PIN_LED1 1
#define PIN_LED2 3
#define PIN_LED3 2

void printSensors();
void controlPIN_LEDs(bool a, bool b, bool c);

/*===========================================
             Define PID instances
===========================================*/
PID_Sum_Uncorrected_cls PID_Sum_Uncorrected1;
PID_SwitchCase_Uncorrected_cls PID_SwitchCase_Uncorrected1;
PID_Sum_Corrected_cls PID_Sum_Corrected1;
PID_SwitchCase_Corrected_cls PID_SwitchCase_Corrected1;
/*===========================================
             Define LED timer
===========================================*/
auto ledTimer = myTimer<5, millis>(); // create a timer with default settings
bool redLedsOff(void *)
{
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED3, LOW);
    return false;
}
bool greenLedOff(void *)
{
    digitalWrite(PIN_LED2, LOW);
    return false;
}
bool xx = true;
bool toggle_pinLeds(void *)
{
    if (xx)
    {
        controlPIN_LEDs(1, 1, 1);
        xx = false;
    }
    else
    {
        controlPIN_LEDs(0, 0, 0);
        xx = true;
    }
    return true;
}
/*===========================================
    Wait for second button function
===========================================*/
void waitForSecondButton()
{
    pinMode(32, INPUT_PULLUP);
    while (digitalRead(32))
    {
        DEBUG_LOGLN("hola");
    }
}
/*===========================================
             Global Variables
===========================================*/
unsigned int lastTime = millis();
unsigned int lastEncL = 0;
unsigned int lastEncR = 0;
unsigned int lastTicks = 0;
/*===========================================
             Setup Function
===========================================*/
void setupPIN_LEDs()
{
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);

    controlPIN_LEDs(0, 0, 0);
}

bool toggle_led(void *)
{
    digitalWrite(2, !digitalRead(2)); // toggle the LED
    return true;                      // repeat? true
}

void setup()
{
    DEBUG_LOG_BEGIN(monitor_speed);
    setupEncoders();
    setupMotors();
    setupSensors();
    setupSpeedChange();
    setupPIN_LEDs();
    // setupVL53L0X();
    pinMode(2, OUTPUT); // set LED pin to OUTPUT

    // call the toggle_led function every 1000 millis (1 second)
    // timer.every(1000000, toggle_led);
    ledTimer.begin();
    ledTimer.every(1000, toggle_pinLeds);
    
    // controlPIN_LEDs(1, 1, 1);

    // waitForSecondButton();
    // controlPIN_LEDs(0, 0, 0);
    DEBUG_LOGLN("setup done");
    // startSpeedChange(&PID_Sum_Corrected1, 0, 0, 0, 0);
    PID_Sum_Uncorrected1.setKp_kd_min_max(1.2, 0.2, -50, 180);
    PID_Sum_Uncorrected1.setBaseSpeed(120);

    PID_Sum_Corrected1.setBaseSpeed(120);
    PID_Sum_Corrected1.setKp_kd_min_max(1.2, 0.2, -150, 250);

    // PID_Sum_Corrected2.setBaseSpeed(120);
    // PID_Sum_Corrected2.setKp_kd_min_max(1.2, 0.5, -140, 160);

    PID_SwitchCase_Uncorrected1.setKp_kd_min_max(30, 4, -140, 180);
    PID_SwitchCase_Uncorrected1.setBaseSpeed(120);

    PID_SwitchCase_Corrected1.setBaseSpeed(120);
    PID_SwitchCase_Corrected1.setKp_kd_min_max(30, 2, -140, 180);

    // PID_SwitchCase_Corrected2.setBaseSpeed(190);
    // PID_SwitchCase_Corrected2.setKp_kd_min_max(25, 1, -150, 250);
    // setupGPIOViewer();
}

/*===========================================
             Loop Function
===========================================*/
int n = 690; // test sensors
// int n = 692; // test motors
// int n = 693; // test encodeur
// int n = 694; // test PID
// int n = 695; // test motors+encoders
// int n = 696; // test Acceleration
// int n = 700; // test pin leds
// int n = 100; // debugging
// int n = 201; // test distance sensor
// int n = -1; // start of maquette
// int n = 3000;

void loop()
{

    unsigned int currentTime = millis();
    unsigned int currentEncL = get_encL();
    unsigned int currentEncR = get_encR();
    readSensors();

    /*__________________________________MAQUETTE_____________________________________________________*/
    if (n == -1)
    {
        setMotors(120, 120);
        if ((dataSensor != 0b11111111) && (currentTime - lastTime > 200))
        {
            n++;
            lastTime = millis();
            reset_encoders();
            PID_Sum_Corrected1.setBaseSpeed(200);
            // PID_Sum_Corrected1.setKp_kd_min_max(2, 0.5, 90, 220);
            PID_Sum_Corrected1.setKp_kd_min_max(2, 0.2, 0, 200);
        }
    }
    else if (n == 0)
    {
    }
    else if (n == 1)
    {
    }
    /*__________________________________DEBUGGING_______________________________________________________*/
    if (n == 200)
    {
        setMotors(-10, -10);
        delay(50);
        setMotors(0, 0);
        n = 201;
    }
    if (n == 201)
    {
    }
    if (n == 100)
    {
        setMotors(0, 0);
        delay(50);
        setMotors(0, 0);
        lastTime = millis();
        while ((get_encL() || get_encR()))
        {
            reset_encL();
            reset_encR();
            delay(500);
        }
        n = 102;
    }
    if (n == 101)
    {
        setMotors(0, 0);
        delay(500);
        n = 102;
    }
    if (n == 102)
    {
        if ((millis() - lastTime) > 500 && (millis() - lastTime) < 1000)
        {
            digitalWrite(PIN_LED1, HIGH);
            digitalWrite(PIN_LED2, HIGH);
            digitalWrite(PIN_LED3, HIGH);
        }
        else if ((millis() - lastTime) > 1000)
        {
            digitalWrite(PIN_LED1, LOW);
            digitalWrite(PIN_LED2, LOW);
            digitalWrite(PIN_LED3, LOW);
            lastTime = millis();
        }
        DEBUG_LOG("Left Encoder: " + String(currentEncL) + " Right Encoder: " + String(currentEncR) + "\n");
    }
    /*__________________________________TESTs_______________________________________________________*/
    // test Motors+Encoders
    if (n == 695)
    {
        delay(1000);
        while (get_encL() < 103 * 1)
        {
            DEBUG_LOG("Left Encoder: " + String(get_encL()) + " Right Encoder: " + String(get_encR()) + "\n");
            setMotors(150, 0);
        }
        DEBUG_LOG("Left Encoder: " + String(get_encL()) + " Right Encoder: " + String(get_encR()) + "\n");
        setMotors(0, 0);
        delay(1000);
        // ***********************
        reset_encoders();

        while (get_encR() < 103 * 1)
        {
            DEBUG_LOG("Left Encoder: " + String(get_encL()) + " Right Encoder: " + String(get_encR()) + "\n");

            setMotors(0, 150);
        }
        DEBUG_LOG("Left Encoder: " + String(get_encL()) + " Right Encoder: " + String(get_encR()) + "\n");
        setMotors(0, 0);
        delay(1000);
        n = 3000;
    }
    // test pid
    if (n == 694)
    {
        _base_pid *testPID = &PID_SwitchCase_Corrected1;
        readSensors();
        testPID->Compute();
        int ll = testPID->lastMoveLeft;
        int rr = testPID->lastMoveRight;
        int err = testPID->lastError;
        DEBUG_LOG("error: " + String(err) + " Left speed: " + String(ll) + " Right Speed: " + String(rr) + "\n");
    }

    // test Motors
    if (n == 692)
    {
        setMotors(160, 163);
        delay(1000);
        setMotors(0, 0);
        delay(1000);
        setMotors(-160, 0);
        delay(1000);
        setMotors(0, 0);
        delay(1000);
        setMotors(0, -160);
        delay(1000);
        setMotors(0, 0);
        delay(1000);
    }
    //-- test sensors
    if (n == 690)
    {
        printSensors();
        delay(500);
    }
    //-- test Encodeur
    if (n == 693)
    {
        if (get_encL() > 106 * 3)
        {
            DEBUG_LOGLN("left wheel made 1 round");
            reset_encL();
        }
        if (get_encR() > 106 * 3)
        {
            DEBUG_LOGLN("right wheel made 1 round");
            reset_encR();
        }
        DEBUG_LOG("Left Encoder: " + String(currentEncL) + " Right Encoder: " + String(currentEncR) + "\n");
    }
    //-- test Acceleration
    if (n == 696)
    {

        if (get_phase() == 0 && !isSpeedChangeRunning())
        {
            DEBUG_LOGLN("phase 0 " + String(PID_Sum_Corrected1.basespeed));
            reset_encoders();
            startSpeedChange(&PID_Sum_Corrected1, 120, 255, 0, 103 * 2 * 2);
        }
        if (get_phase() == 1 && !isSpeedChangeRunning())
        {
            DEBUG_LOGLN("phase 1 " + String(PID_Sum_Corrected1.basespeed));
            startSpeedChange(&PID_Sum_Corrected1, 255, 90, 103 * 3 * 2, 103 * 2 * 2);
        }
        if (get_phase() == 2 && !isSpeedChangeRunning())
        {
            DEBUG_LOGLN("done " + String(PID_Sum_Corrected1.basespeed));
            setMotors(0, 0);
            delay(2000);
            n = 101;
            return;
        }
        PID_Sum_Corrected1.Compute();
        // setMotors(150,150);
    }
    if (n == 700)
    {
        while (1)
        {
            digitalWrite(PIN_LED1, HIGH);
            digitalWrite(PIN_LED2, LOW);
            digitalWrite(PIN_LED3, HIGH);

            delay(1000);
            digitalWrite(PIN_LED1, LOW);
            digitalWrite(PIN_LED2, HIGH);
            digitalWrite(PIN_LED3, LOW);

            delay(1000);
            digitalWrite(PIN_LED1, HIGH);
            digitalWrite(PIN_LED2, HIGH);
            digitalWrite(PIN_LED3, HIGH);

            delay(1000);
            digitalWrite(PIN_LED1, LOW);
            digitalWrite(PIN_LED2, LOW);
            digitalWrite(PIN_LED3, LOW);

            delay(1000);
        }
    }
}

void printSensors()
{
    for (int i = 0; i < SENSOR_COUNT; i++)
    {
        DEBUG_LOG(s[i]);
    }
    DEBUG_LOGLN(" // somme: " + String(somme) + " cnt: " + String(cnt));
}

void controlPIN_LEDs(bool a, bool b, bool c)
{
    digitalWrite(PIN_LED1, a ? HIGH : LOW);
    digitalWrite(PIN_LED2, b ? HIGH : LOW);
    digitalWrite(PIN_LED3, c ? HIGH : LOW);
}
#ifdef ENABLE_DISTANCE_SENSOR
void testDistanceSensor()
{
    unsigned long dt = micros();
    int dist = lox.readRange();
    dt = micros() - dt;
    DEBUG_LOGLN("dt:" + String(dt) + " --  Distance (mm): " + String(dist));
    // PID_Distance.Compute(dist);
    // delay(1000);
}
#else
void testDistanceSensor()
{
    DEBUG_LOGLN("Distance sensor not enabled");
}
#endif
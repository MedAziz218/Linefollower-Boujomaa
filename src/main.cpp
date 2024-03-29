
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
#include "motor_encoder.h"
#include "pid_classes.h"
#include "speed_change.h"
#include "arduino-timer.h"

#define p2t(x) 103 * 2 * x
// #include "VL53L0X_sensor.h"
/*===========================================
   Conditional Compilation for Debugging
===========================================*/
#define ENABLE_DEBUG_LOGGING 0
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
bool testPIN_LEDS(void *);
void controlPIN_LEDs(bool a, bool b, bool c);
void controlPIN_LEDs(int x);

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
auto ledTimer = Timer<5, millis>(); // create a timer with default settings
const int checkpoint_anim[] = {0b100, 0b010, 0b111, 0b000};
void show_checkpoint(int n)
{
    controlPIN_LEDs(checkpoint_anim[n % 4]);
}
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
        // DEBUG_LOGLN("hola");
    }
    delay(200);
}
/*===========================================
             Global Variables
===========================================*/
SpeedChanger_cls SpeedChanger;
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
    if (ENABLE_DEBUG_LOGGING)
        DEBUG_LOG_BEGIN(monitor_speed);
    else
        setupPIN_LEDs();
    setupEncoders();
    setupMotors();
    setupSensors();

    // setupVL53L0X();
    pinMode(PIN_LED3, OUTPUT); // set LED pin to OUTPUT

    // call the toggle_led function every 1000 millis (1 second)
    // timer.every(1000000, toggle_led);
    // ledTimer.every(1000, toggle_pinLeds);

    controlPIN_LEDs(1, 1, 1);

    waitForSecondButton();
    controlPIN_LEDs(0, 0, 0);
    DEBUG_LOGLN("setup done");

    PID_Sum_Uncorrected1.setKp_kd_min_max(1.2, 0.2, -50, 180);
    PID_Sum_Uncorrected1.setBaseSpeed(120);
    // this is well tuned pid
    //  PID_Sum_Corrected1.setBaseSpeed(120);
    //  PID_Sum_Corrected1.setKp_kd_min_max(1.2, 0.2, -150, 250);
    // starting pid FIXME: uncomment
    PID_Sum_Corrected1.setBaseSpeed(170);
    PID_Sum_Corrected1.setKp_kd_min_max(1.8, 1, -220, 220);

    // PID_Sum_Corrected1.setBaseSpeed(200);
    // PID_Sum_Corrected1.setKp_kd_min_max(2, 0.5, -200, 210);

    // PID_Sum_Corrected2.setBaseSpeed(120);
    // PID_Sum_Corrected2.setKp_kd_min_max(1.2, 0.5, -140, 160);

    PID_SwitchCase_Uncorrected1.setKp_kd_min_max(30, 4, -140, 180);
    PID_SwitchCase_Uncorrected1.setBaseSpeed(120);

    PID_SwitchCase_Corrected1.setBaseSpeed(120);
    PID_SwitchCase_Corrected1.setKp_kd_min_max(30, 2, -140, 180);

    // PID_SwitchCase_Corrected2.setBaseSpeed(190);
    // PID_SwitchCase_Corrected2.setKp_kd_min_max(25, 1, -150, 250);
    // setupGPIOViewer();
    SpeedChanger.begin();
    // SpeedChanger.add(&PID_Sum_Corrected1, 120, 200, 0, 103 * 2 * 0.5);
    // SpeedChanger.add(&PID_Sum_Corrected1, 200, 70, 103 * 2 * 3.5, 103 * 2 * 0.5);
    // SpeedChanger.add(&PID_Sum_Corrected1, 70, 120, 103 * 2 * 8, 103 * 2 * 0.5);
}

/*===========================================
             Loop Function
===========================================*/
// int n = 690; // test sensors
// int n = 692; // test motors
// int n = 693; // test encodeur
// int n = 694; // test PID
// int n = 695; // test motors+encoders
// int n = 696; // test speedchange
// int n = 700; // test pin leds
// int n = 100; // debugging
// int n = 201; // test distance sensor
int n = -1; // start of maquette
// int n= 7;
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
            PID_Sum_Corrected1.setBaseSpeed(150);
            PID_Sum_Corrected1.setKp_kd_min_max(2, 1, -240, 255);
        }
    }
    else if (n == 0)
    {
        if (cnt && somme == 3.5)
        {
            // start ba3d carro
            show_checkpoint(n);
            reset_encoders();
            PID_Sum_Corrected1.setKp_kd_min_max(1.9, 0.75, -240, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 150, 255, p2t(0), p2t(0.5));
            // SpeedChanger.add_apply_when_done(1.9, 0.75, -240, 255);

            SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(3.6), p2t(0.5));
            SpeedChanger.add_apply_when_done(1.2, 0.2, -150, 255);
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 1)
    {
        if (SpeedChanger.isEmpty() && somme <= 2)
        {
            // debut sinsuet
            show_checkpoint(n);
            reset_encoders();

            PID_Sum_Corrected1.setKp_kd_min_max(2, 0.2, -150, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 120, 170, p2t(1), p2t(0.5));
            SpeedChanger.add(&PID_Sum_Corrected1, 170, 120, p2t(2), p2t(0.5));

            // SpeedChanger.add(&PID_Sum_Corrected1, 170, 50, 103 * 8 * 3, 103 * 2 * 1);
            n++;
         
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 2)
    {
        if (SpeedChanger.isEmpty()&& somme >=5)
        {
            show_checkpoint(n);
            reset_encoders();
            SpeedChanger.add(&PID_Sum_Corrected1, 120, 170, p2t(1), p2t(0.5));
            SpeedChanger.add(&PID_Sum_Corrected1, 170, 120, p2t(2), p2t(0.5));
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    
    else if (n == 3)
    {
        if (SpeedChanger.isEmpty() )
        {   
            reset_encoders();
            while (!(cnt && somme ==3.5 ) || get_encoders()<p2t(4) ){
                readSensors();
                PID_Sum_Corrected1.Compute();
            }
            reset_encoders();
            
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 4)
    {

        if (SpeedChanger.isEmpty() && somme <= 2)
        {
            // dora 1 ba3d sinuset (isar)
            show_checkpoint(n);
            reset_encoders();
            while (!(cnt && somme == 3.5))
            {
                readSensors();
                PID_Sum_Corrected1.Compute();
            }

            reset_encoders();
            PID_Sum_Corrected1.setKp_kd_min_max(1.9, 0.75, -240, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 120, 255, p2t(0), p2t(0.5));
            SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(2.0), p2t(0.5));
            SpeedChanger.add_apply_when_done(2, 0.2, -150, 220);
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 5)
    {
        if (SpeedChanger.isEmpty() && somme >= 5)
        {
            if (SpeedChanger.isEmpty() && somme >= 5)
            {
                // dora 2 ba3d sinuset (imin)

                show_checkpoint(n);
                reset_encoders();
                while (!(cnt && somme == 3.5))
                {
                    readSensors();
                    PID_Sum_Corrected1.Compute();
                }

                reset_encoders();
                PID_Sum_Corrected1.setKp_kd_min_max(1.9, 1, -240, 255);
                SpeedChanger.add(&PID_Sum_Corrected1, 120, 255, p2t(0), p2t(0.5));
                SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(2.9), p2t(0.5));
                SpeedChanger.add_apply_when_done(2, 0.2, -150, 220);
                n++;
            }
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 6)
    {
        if (SpeedChanger.isEmpty() && somme >= 5)
        {
            // dora 5ayba ( U turn )
            reset_encoders();
            while (!(cnt && somme == 3.5 && get_encoders() > p2t(2)))
            {

                readSensors();
                PID_Sum_Corrected1.Compute();
            }
            show_checkpoint(n);
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 7)
    {
        if (cnt && somme == 3.5)
        {
            // start ba3d carro
            show_checkpoint(n);
            reset_encoders();
            PID_Sum_Corrected1.setKp_kd_min_max(1.9, 0.75, -240, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 150, 255, p2t(0), p2t(0.5));
            // SpeedChanger.add_apply_when_done(1.9, 0.75, -240, 255);

            SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(2), p2t(0.5));
            SpeedChanger.add_apply_when_done(1.2, 0.2, -150, 250);
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 8)
    {
        if (SpeedChanger.isEmpty() && somme <= 2)
        {
            // dora 1 ba3d sinuset (isar)
            show_checkpoint(n);
            reset_encoders();
            while (!(cnt && somme == 3.5))
            {
                readSensors();
                PID_Sum_Corrected1.Compute();
            }

            reset_encoders();
            PID_Sum_Corrected1.setKp_kd_min_max(1.9, 0.75, -240, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 120, 255, p2t(0), p2t(0.5));
            SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(1), p2t(0.5));
            SpeedChanger.add_apply_when_done(2, 0.2, -150, 220);
            n++;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 9)
    {
        if (SpeedChanger.isEmpty() && somme <= 2)
        {
            reset_encoders();
            SpeedChanger.add(&PID_Sum_Corrected1, 120, 170, p2t(1), p2t(0.5));
            SpeedChanger.add_apply_when_done(1.9, 0.75, -240, 255);
            SpeedChanger.add(&PID_Sum_Corrected1, 170, 255, p2t(1.55), p2t(0.5));
            SpeedChanger.add(&PID_Sum_Corrected1, 255, 120, p2t(8), p2t(0.5));
            n++;
            
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }
    else if (n == 10)
    {
        if (SpeedChanger.isEmpty())
        {
            reset_encoders();

            n = 100;
        }
        else
        {
            PID_Sum_Corrected1.Compute();
        }
    }


    /*__________________________________DEBUGGING_______________________________________________________*/

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
        _base_pid *testPID = &PID_Sum_Corrected1;
        readSensors();
        testPID->Compute();
        // int ll = testPID->lastMoveLeft;
        // int rr = testPID->lastMoveRight;
        // int err = testPID->lastError;
        // DEBUG_LOG("error: " + String(err) + " Left speed: " + String(ll) + " Right Speed: " + String(rr) + "\n");
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
    //-- test speedchange
    if (n == 696)
    {

        // TODO: add speed change test code
        PID_Sum_Corrected1.Compute();
        // setMotors(150,150);
    }
    if (n == 700)
    {
        ledTimer.cancel();
        ledTimer.every(45, testPIN_LEDS);
        n = 690;
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

void controlPIN_LEDs(int x)
{
    bool a = x & 0b100;
    bool b = x & 0b010;
    bool c = x & 0b001;
    controlPIN_LEDs(a, b, c);
}
void controlPIN_LEDs(bool a, bool b, bool c)
{

    digitalWrite(PIN_LED1, a ? HIGH : LOW);
    digitalWrite(PIN_LED2, b ? HIGH : LOW);
    digitalWrite(PIN_LED3, c ? HIGH : LOW);
}
int test_PIN_LEDS_Var = 0;
bool testPIN_LEDS(void *)
{

    // clang-format off
    byte anim[] = {
        0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000,
        0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000,
        0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000,
        0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000,
        
        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,
        0b000, 0b000, 0b001, 0b001, 0b011, 0b011, 0b111, 0b111, 0b110, 0b110, 0b100, 0b100, 0b000, 0b000,
        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,
        0b000, 0b000, 0b001, 0b001, 0b011, 0b011, 0b111, 0b111, 0b110, 0b110, 0b100, 0b100, 0b000, 0b000,
        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,

        0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000, 0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 
        0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000, 0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 
        0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000, 0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 
        0b000, 0b001, 0b011, 0b111, 0b110, 0b100, 0b000, 0b000, 0b100, 0b110, 0b111, 0b011, 0b001, 0b000, 
        
        0b000, 0b000, 0b001, 0b001, 0b011, 0b011, 0b111, 0b111, 0b110, 0b110, 0b100, 0b100, 0b000, 0b000,
        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,

        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,
        0b000, 0b000, 0b100, 0b100, 0b110, 0b110, 0b111, 0b111, 0b011, 0b011, 0b001, 0b001, 0b000, 0b000,
        
        0b000, 0b000, 0b001, 0b001, 0b011, 0b011, 0b111, 0b111, 0b110, 0b110, 0b100, 0b100, 0b000, 0b000,
        0b000, 0b000, 0b001, 0b001, 0b011, 0b011, 0b111, 0b111, 0b110, 0b110, 0b100, 0b100, 0b000, 0b000,
        
        0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000,
        0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 0b111, 
        
        0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000,
        
        0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 
        0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 
        
        0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000,
                
        0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 0b010, 
        0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 0b101, 
        
        0b000, 0b000, 0b000, 0b000, 0b000, 0b000, 0b000,

        0b111, 0b111, 0b111, 
        0b000, 0b000, 0b000,
        0b111, 0b111, 0b111, 
        0b000, 0b000, 0b000,};
    // clang-format on
    controlPIN_LEDs(anim[test_PIN_LEDS_Var]);
    test_PIN_LEDS_Var++;
    if (test_PIN_LEDS_Var >= sizeof(anim) / sizeof(anim[0]))
    {
        test_PIN_LEDS_Var = 0;
    }
    return true;
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
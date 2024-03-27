#ifndef SPEED_CHANGE_H
#define SPEED_CHANGE_H
#include "Arduino.h"
#include "pid_classes.h"
#include "bounded_queue.h"
#include "esp_timer.h"
/*===========================================
    Define Speed Change Data and Timer
===========================================*/

struct _speedChangeTask
{
    _base_pid *chosen_pid_instance;
    int startpeed, endspeed;
    int startdist, enddist;
};

class SpeedChanger_cls
{
private:
    BoundedQueue<_speedChangeTask, 6> speedChangeQueue;
    bool _change_in_progress = false;
    _speedChangeTask current_task;
    bool _update_speed_tick(void)
    {
        // x++;
        _base_pid *chosen_pid_instance = current_task.chosen_pid_instance;
        int currentspeed = chosen_pid_instance->basespeed;
        int nextspeed;
        unsigned int dist = get_encoders();
        // unsigned int dist = millis();

        int startpeed = current_task.startpeed, endspeed = current_task.endspeed;
        int startdist = current_task.startdist, enddist = current_task.enddist;
        nextspeed = map(dist, startdist, enddist, startpeed, endspeed);
        nextspeed = constrain(nextspeed, min(startpeed, endspeed), max(startpeed, endspeed));
        chosen_pid_instance->setBaseSpeed(nextspeed);
        if (nextspeed == endspeed)
        {
            // DEBUG_PRINT("End Acceleration: " + String(chosen_pid_instance->basespeed)+" millis: "+String(dist) );
            // DEBUG_PRINTLN(" x " + String(x));
            _change_in_progress = false;
            return false;
        }
        // DEBUG_PRINT("Acceleration: " + String(chosen_pid_instance->basespeed)+" millis: "+String(dist));
        // DEBUG_PRINTLN(" x " + String(x));
        return true;
    }
    void tick()
    {
        if (speedChangeQueue.isEmpty())
            return;
        if (_change_in_progress)
        {
            _change_in_progress = _update_speed_tick();
            if (!_change_in_progress && !speedChangeQueue.isEmpty())
                speedChangeQueue.dequeue();
        }
        if (!_change_in_progress && !speedChangeQueue.isEmpty())
        {
            current_task = speedChangeQueue.peek();
            _change_in_progress = _update_speed_tick();
        }
    }
    static void taskEntryPoint(void *pvParameters)
    {
        SpeedChanger_cls *Instance = static_cast<SpeedChanger_cls *>(pvParameters);
        if (Instance)
        {
            Instance->tick();
        }
    }

public:
    bool started = false;
    void begin()
    {
        if (started)
            return;
        started = true;
        esp_timer_handle_t timer_handle;
        esp_timer_create_args_t timer_args = {
            .callback = &SpeedChanger_cls::taskEntryPoint,
            .arg = this,
            .name = "MyTimerTask"};
        esp_timer_create(&timer_args, &timer_handle);
        // Start the timer to trigger the callback function every  (100 microseconds)
        esp_timer_start_periodic(timer_handle, 500);
    }
    void add(_base_pid *_pid, int _startpeed, int _endspeed, int _startdist, int _durationdist)
    {
        if (!speedChangeQueue.isFull())
        {
            speedChangeQueue.enqueue({_pid, _startpeed, _endspeed, _startdist, _startdist + _durationdist});
        }
    };
};
void setupSpeedChange();

#endif
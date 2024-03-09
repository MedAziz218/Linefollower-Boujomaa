// #ifndef INC_FREERTOS_H
// #include <freertos/FreeRTOS.h>
// #endif
// #ifndef INC_TASK_H
// #include <freertos/task.h>
// #endif
#ifndef MY_TIMER_H
#define MY_TIMER_H
#include <Arduino.h>
#include "arduino-timer.h"
#include "esp_timer.h"

template <
    size_t max_tasks,  /* max allocated tasks */
    unsigned long (*time_func)() /* time function for timer */
    >
class myTimer : public Timer<max_tasks, time_func>
{
public:
    void begin()
    {
        // TickType_t tickRate = configTICK_RATE_HZ;
        // xTaskCreatePinnedToCore(&myTimer::taskEntryPoint, "MyTimerTask", 1024, this, 1, NULL, 0);
        // xTaskCreatePinnedToCore(&myTimer::taskEntryPoint, "MyTimerTask", 1024, this, 1, NULL, 0);
        // xTaskCreate(&myTimer::taskEntryPoint, "MyTImerTick", 2048, this, 1, NULL);
        // Serial.printf("Tick rate: %u Hz\n", tickRate);
        if (started) return;
        started = true;
        esp_timer_handle_t timer_handle;
        esp_timer_create_args_t timer_args = {
            .callback = &myTimer::taskEntryPoint,
            .arg = this,
            .name = "MyTimerTask"};
        esp_timer_create(&timer_args, &timer_handle);
        // Start the timer to trigger the callback function every  (100 microseconds)
        esp_timer_start_periodic(timer_handle, 100);
    }

private:
    bool started = false;
    static void taskEntryPoint(void *pvParameters)
    {

        myTimer *timerInstance = static_cast<myTimer *>(pvParameters);
        if (timerInstance)
        {
            timerInstance->taskFunction();
        }
    }

    void taskFunction()
    {
        this->tick();
    }
};

#endif
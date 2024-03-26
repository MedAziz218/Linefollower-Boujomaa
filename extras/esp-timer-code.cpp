
#include "esp_timer.h"
#include "Arduino.h"
void task(void *pvParameters){
    String *str = static_cast<String *>(pvParameters);
    Serial.println("Timer fired" + *str);
}
void test()
{

    String str = "anything";
    esp_timer_handle_t timer_handle;
    esp_timer_create_args_t timer_args = {
        .callback = &task,
        .arg = &str,
        .name = "MyTimerTask"};
    esp_timer_create(&timer_args, &timer_handle);
    // Start the timer to trigger the callback function every  (100 microseconds)
    esp_timer_start_periodic(timer_handle, 100);
}
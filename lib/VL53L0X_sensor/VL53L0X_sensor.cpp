#include "vl53l0x_sensor.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setupVL53L0X()
{
  Serial.println(" VL53L0X test");
  if (!lox.begin())
  {
    Serial.println(F("Failed to boot VL53L0X"));
    unsigned long lastTime = millis();
    while (1)
    {
      if (millis() - lastTime > 1000)
      {
        Serial.println("VL53L0X error");
        lastTime = millis();
        
      }
    }
  }
}
uint16_t  readVL53L0X(){
    return lox.readRange();
}

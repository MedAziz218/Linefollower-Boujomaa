#include "sensorsTCRT.h"

const byte posSensor[SENSOR_COUNT] = {16, 17, 5, 18, 19, 15, 4, 23};
bool s[8] = {0};
int dataSensor = 0b00000000;
float lastOnLineSomme = 0;
float somme = 0;
int cnt = 0;
bool onLine = 0;
bool lineColor = BLACK_LINE;

void setupSensors()
{
  for (int i = 0; i < SENSOR_COUNT; i++)
    pinMode(posSensor[i], INPUT);
}
void readSensors()
{
  memset(&s, 0, sizeof(s)); //  s[8]= {0};
  dataSensor = 0b00000000;

  for (int x = 0; x < SENSOR_COUNT; x++)
  {
    if (digitalRead(posSensor[x]))
    {
      dataSensor = dataSensor + (0b10000000 >> x);
      s[x] = 1;
    }
  }
  if (lineColor == WHITE_LINE)
  {
    for (int i = 0; i < SENSOR_COUNT; i++)
      s[i] = !s[i];
    dataSensor = 0b11111111 - dataSensor;
  }

  onLine = 0;
  cnt = 0;
  somme = 0;
  for (int i = 0; i < SENSOR_COUNT; i++)
  {
    if (s[i])
    {
      cnt++;
      somme += i;
    }
  }
  if (cnt)
  {
    somme = somme / cnt;
    onLine = 1;
    if (somme != 3.5)
      lastOnLineSomme = somme;
  }
}


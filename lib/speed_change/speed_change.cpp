#include "speed_change.h"
struct
{
  _base_pid *chosen_pid_instance;
  int startpeed, endspeed;
  int startdist, enddist;
  int phase = 0;
  bool inProgress = false;

} speedChangeData;

Timer<1, micros> speedChangeTimer; // create a timer with 1 task and microsecond resolution

bool _update_speed_step(void *)
{
  // x++;
  _base_pid *chosen_pid_instance = speedChangeData.chosen_pid_instance;
  int currentspeed = chosen_pid_instance->basespeed;
  int nextspeed;
  unsigned int dist = get_encoders();
  // unsigned int dist = millis();

  int startpeed = speedChangeData.startpeed, endspeed = speedChangeData.endspeed;
  int startdist = speedChangeData.startdist, enddist = speedChangeData.enddist;
  nextspeed = map(dist, startdist, enddist, startpeed, endspeed);
  nextspeed = constrain(nextspeed, min(startpeed, endspeed), max(startpeed, endspeed));
  chosen_pid_instance->setBaseSpeed(nextspeed);
  if (nextspeed == endspeed)
  {
    // DEBUG_PRINT("End Acceleration: " + String(chosen_pid_instance->basespeed)+" millis: "+String(dist) );
    // DEBUG_PRINTLN(" x " + String(x));
    speedChangeData.phase++;
    speedChangeData.inProgress = false;
    return false;
  }
  // DEBUG_PRINT("Acceleration: " + String(chosen_pid_instance->basespeed)+" millis: "+String(dist));
  // DEBUG_PRINTLN(" x " + String(x));
  return true;
}
bool isSpeedChangeRunning()
{
  return !speedChangeTimer.empty() && speedChangeData.inProgress;
}
void reset_phase()
{
  speedChangeData.phase = 0;
}
int get_phase()
{
  return speedChangeData.phase;
}
void startSpeedChange(_base_pid *_pid, int _startpeed, int _endspeed, int _startdist, int _durationdist)
{
  if (isSpeedChangeRunning())
  {
    speedChangeTimer.cancel();
  }
  speedChangeData.inProgress = true;
  speedChangeData.chosen_pid_instance = _pid;
  speedChangeData.startpeed = _startpeed;
  speedChangeData.endspeed = _endspeed;
  speedChangeData.startdist = _startdist;
  speedChangeData.enddist = _startdist + _durationdist;
  speedChangeTimer.every(1, _update_speed_step);
}
void setupSpeedChange()
{
  
}
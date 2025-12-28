#ifndef ACTUATORMANAGER_H
#define ACTUATORMANAGER_H
#include <Arduino.h>

struct Actuator {
  String Name = "-AA";
  String Name0 = "-AA";
  int pin = -1;         // The GPIO pin number (e.g., D5, GPIO14)
  bool pwm = false;     // if true is a pwm act otherwise is an on / off
  int duty = 128;       // The current duty cycle value (0 to range_max)
  int dutyMax = 1023;   // The maximum duty cycle value (often 255 or 1023)
  int freq = 1200;        // pwm freq
  bool active = false;  // sensor active
  bool action = true;   // sensor active when Pin + , false when Pin- for digital actuator

  void Init();
  void Activate();
  void deActivate();
  void Reset();
  void PowON(int dt);  //set a pwm duty dt or digi actuator on
  void PowOFF();       // power off

  void rampLinear(int dtt);  // liniar rampingtotarget from current
  void rampExpCurve(int dtt);
  void rampSCurve(int dtt);
};


extern Actuator LED;
extern Actuator Buzzer;
extern Actuator Buzzer_G;
extern Actuator Relay;

#endif  //ACTUATORMANAGER_H
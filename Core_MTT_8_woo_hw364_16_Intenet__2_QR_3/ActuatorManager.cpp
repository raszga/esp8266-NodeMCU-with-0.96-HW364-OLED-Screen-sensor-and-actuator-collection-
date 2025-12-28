#include "ActuatorManager.h"


/*============================================================================
struct Actuator {
  String Name = "";
  String Name0 = "";
  int pin = -1;         // The GPIO pin number (e.g., D5, GPIO14)
  bool pwm = false;     // if true is a pwm act otherwise is an on / off
  int duty = 125;       // The current duty cycle value (0 to range_max)
  int dutyMax = 1023;   // The maximum duty cycle value (often 255 or 1023)
  int freq = 100;       // pwm freq
  bool active = false;  // sensor active
  bool action = true;   // sensor active when Pin + , false when Pin- for digital actuator
/*============================================================================*/
void Actuator::Init() {
  Name0 = Name;
  Name += String(ESP.getChipId());
  active = true;
  if (pin != -1) {
    pinMode(pin, OUTPUT);
    if (pwm) {
      analogWriteFreq(freq);
      analogWriteRange(dutyMax);
    }
    PowON(0);
    active = false;
  }
}
/*============================================================================*/
void Actuator::Activate() {
  active = true;
}
/*============================================================================*/
void Actuator::deActivate() {
  active = false;
}
/*============================================================================*/
void Actuator ::PowON(int dt) {
  if (pwm && active) {
    if (dt > dutyMax) dt = dutyMax;
    analogWrite(pin, dt);
  } else {
    digitalWrite(pin, (dt > 0) ^ !action);  //keep an eye on int
  }
}
/*============================================================================*/
void Actuator ::PowOFF() {
  PowON(0);
}
/*============================================================================*/
void Actuator::Reset() {
  PowOFF();
  Name = Name0;
  deActivate();
}
/*============================================================================*/
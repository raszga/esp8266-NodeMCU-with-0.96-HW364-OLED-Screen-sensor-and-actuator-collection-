#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include <Arduino.h>

extern const unsigned long ALARM_PULSE_DURATION_MS;
extern const unsigned long PULSE_TIMEOUT_MIN;
extern const unsigned long UPTIME_RESET_MIN;
extern const String ESPid;
extern unsigned long int pulsarTime;
// --- SENSOR STRUCTURE & DEFINITIONS ---
struct Sensor {
  String Name = "-1XXX";   // sensor name
  int Pin = -1;            // digital pin
  int Set = INPUT_PULLUP;  // input type
  int Norm = 1;            // normal stage
  bool analogue = false;   // if is an analogue pin
  int Vmin = 0;            // minim analog value
  int Vmax = 4095;         // max analog value ot max pulser time for soft sensor
  int V = -1;              // curent value
  int Pow = -1;            // power pin if neede
  int normPow = 0;         // no power feed stage
  int active = false;      // if sensor is active
  bool soft = false;       // sotware sensor
  bool softStage = true;   // soft sensor stage
  String sMSG = "-1XXX";   // Sensor message
  String Name0 = Name;     //  preserve initial name for Reset

  // Sensor Methods.........................
  bool (*pointerCheck)(int);
  void Init();        // init function
  void Reset();       // reset function
  bool Check();       // digital check
  void Read();        // digital read
  void PowON();       // power pin on
  void PowOFF();      // power pin off
  void Activate();    // activate sensor
  void deActivate();  // deactivate sensor
  String Response();  // Sensor string Response
};                    // Sensor end------------------------------------------------------
/* sENSORS EXTERNAL DECLARATIONS==================================================*/
extern Sensor TimiButton;
extern Sensor AnalogSensor;
extern Sensor LockDoor;
extern Sensor isInternet;

/* Prototipe bolean function for pointer function================================*/
bool checkProto(int x);
bool CheckPulsar(int Tmax);

#endif  // SENSORHANDLER_H
/*======================================EOF======================================*/
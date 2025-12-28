#include "SensorHandler.h"
// --- CONSTANT DEFINITIONS (Defined here once using global linkage) ---
const unsigned long ALARM_PULSE_DURATION_MS = 100;
const unsigned long PULSE_TIMEOUT_MIN = 6 * 60 * 1000;
const unsigned long UPTIME_RESET_MIN = 10 * 60 * 1000;
ADC_MODE(ADC_VCC);  // set the ESP.getVcc mode
const String ESPid = "ESP" + String(ESP.getChipId());
unsigned long int pulsarTime = millis();

void Sensor::Init() {
  Name0 = Name;
  //Name += String(ESP.getChipId());
  V = -1;
  if (soft) {
    pointerCheck = checkProto;
    active = false;
    return;
  }
  if (!analogue) {
    pinMode(Pin, Set);  // pin input for sensor stage
    if (Pow != -1) {
      pinMode(Pow, OUTPUT);        // power pin output
      digitalWrite(Pow, normPow);  // no power yet
    }
  }
  if (analogue) {
    if (Pow != -1) {
      pinMode(Pow, OUTPUT);        // power pin output
      digitalWrite(Pow, normPow);  // no power yet
    }
  }
  active = false;
  return;
}
/*============================================================================*/
void Sensor ::PowON() {
  if ((active) && (Pow != -1)) {
    digitalWrite(Pow, !normPow);
  }
  return;
}
/*============================================================================*/
void Sensor ::PowOFF() {
  if ((active) && (Pow != -1)) {
    digitalWrite(Pow, normPow);
  }
  return;
}
/*============================================================================*/
void Sensor ::Activate() {
  active = true;
  return;
}
/*============================================================================*/
void Sensor ::deActivate() {
  PowOFF();
  active = false;
  return;
}
/*============================================================================*/
void Sensor ::Read() {
  V = -1;
  if (soft) {
    softStage, V = pointerCheck(Vmax);
    return;
  }
  PowON();
  if (analogue && active) {
    while (V == -1) {
      delay(10);
      V = ESP.getVcc();  // read analog value typical ESP8266
    }
  }
  if (!analogue && active) {
    while (V == -1) {
      delay(10);
      V = digitalRead(Pin);
    }
  }
  return;
}
/*============================================================================*/
bool Sensor ::Check() {
  bool result = false;
  Read();
  if (soft) {
    result = softStage;
    return result;
  }
  if (analogue) {
    result = ((Vmin <= V) && (V <= Vmax));
    return result;
  }
  if (!analogue) {
    result = (V == Norm);
    return result;
  }
  return result;
}
/*============================================================================*/
String Sensor::Response() {
  Read();
  sMSG = Name + ":" + String(V) + ":";
  if (Check()) { sMSG += "alarmOFF"; }
  if (!Check()) { sMSG += "alarmON"; }
  return sMSG;
}
/*============================================================================*/
void Sensor::Reset() {
  Name = Name0;
  PowOFF();
  active = false;
  V = -1;
}
/* Prototipe bolean function for pointer function================================*/
bool checkProto(int x) {
  static int y = x;
  return true;
}
/********************************specific functions******************************/
bool CheckPulsar(int Tmax) {
  int dif = int((millis() - pulsarTime) / 1000);
  if (dif <= Tmax) {
    return true;
    //Serial.println(dif);
  } else {
    //Serial.println(dif);
    return false;
  }
}
/*********************** End Of File *******************************************/
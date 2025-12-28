#include <Wire.h>  // I2C communication library
#include "OLED_Utilities.h"
#include "MqttHandler.h"
#include "SensorHandler.h"
#include "ActuatorManager.h"
// --- WIFI CREDENTIALS ---
/* Credentials ----------------------------------------------------------------


-------------------------------------------------------------------------------*/
const char* MY_WIFI_SSID = "A&C_2GHz"; // put your values here
const char* MY_WIFI_PASSWORD = "01358";
// Create an instance of the MqttHandler class
MqttHandler mqttHandler;
/*============================================================================*/
Sensor TimiButton = { .Name = "Button-", .Pin = 15, .Norm = 0 };
Sensor AnalogSensor = { .Name = "Water-", .analogue = true };
Sensor isInternet = { .Name = "Pulsar-", .Vmax = 700, .soft = true, .pointerCheck = CheckPulsar };
/*============================================================================*/
Actuator LED{ .Name = "LED-", .pin = 2, .pwm = true };  //2
Actuator Buzzer{ .Name = "Buzz-", .pin = 5, .pwm = true };
Actuator Buzzer_G{ .Name = "Buzz-G-", .pin = 4 };  // this is only for convenience
/************SETUP fUNCTION*****************************************************/
void setup() {
  Serial.begin(9600);
  delay(100);

  TimiButton.Init();
  TimiButton.Activate();

  AnalogSensor.Init();
  AnalogSensor.Activate();

  isInternet.Init();
  isInternet.pointerCheck = CheckPulsar;
  isInternet.Activate();

  LED.Init();
  LED.Activate();
  LED.PowON(0);

  Buzzer.Init();
  Buzzer.Activate();
  Buzzer.PowON(0);

  Buzzer_G.Init();
  Buzzer_G.Activate();
  Buzzer_G.PowON(0);

  Wire.begin(14, 12);  // Start I2C bus FOR THIS PARTICULAR oled

  if (!oled.begin()) {
    Serial.println(F("OLED initialization failed!"));
    while (true)
      ;  // Freeze
  }

  oled.printLine(String(ESP.getChipId()).c_str(), 0, 2);
  mqttHandler.begin(MY_WIFI_SSID, MY_WIFI_PASSWORD);
  delay(100);
  Buzzer.PowON(500);
  delay(100);
  Buzzer.PowON(0);
  oled.genQR("Timisoara 1989!", 1000);
}
/*==========================================================================-*/
void loop() {
  mqttHandler.handleMqttLoop();

  Buzzer.PowON(0);
  LED.PowON(0);

  if (message.indexOf("cls") != -1) {
    oled.clear();
  }

  if (message.indexOf("Diff_ON") != -1) {
    Buzzer.PowON(256);
  }

  if (message.indexOf("Diff_FULL") != -1) {
    Buzzer.PowON(1023);
  }

  if (message.indexOf("Diff_OFF") != -1) {
    Buzzer.PowON(0);
    //oled.printLine(String(ESP.getChipId()).c_str(), 0, 1);
  }
  if (!isInternet.Check()) {
    mqttHandler.publish(isInternet.Response());
  }
  oled.printLine(String(millis() / 1000).c_str(), 3, 1);
  if (message != "") {
    Serial.println(message);
    oled.clear();
    oled.printLine(ESPid.c_str(), 0, 2);  // ESP id no
    oled.printLine(message.c_str(), 2, 1);
    oled.printLine(TimiButton.Response().c_str(), 5, 1);
    oled.printLine(AnalogSensor.Response().c_str(), 6, 1);
    oled.drawProgressBar(60, 128, 4, int((100 * AnalogSensor.V) / AnalogSensor.Vmax));
    if (message.indexOf("QRcode") != -1) {
      oled.genQR("Timisoara_1989!\n\r", 10000);
    }
    if (message.indexOf(">>PULSE-ER") != -1) {
      Serial.println(millis() - pulsarTime);
      pulsarTime = millis();  // reset timer for pulsarTime
      oled.printLine(String(millis() / 1000).c_str(), 3, 1);
    }

    if (!isInternet.Check()) {
      mqttHandler.publish(isInternet.Response());
      Buzzer.PowON(512);
    }

    if (message.indexOf("alarmON") != -1) {
      oled.printLine("------!!!!!---------", 2, 1);
      oled.printLine(message.c_str(), 3, 1);
      LED.PowON(512);
      Buzzer.PowON(512);
      mqttHandler.publish("alarmOFF");
      delay(100);
      Buzzer.PowON(0);
      LED.PowON(0);
    }
    if (message.indexOf("Check") != -1) {
      LED.PowON(512);
      Buzzer.PowON(256);
      mqttHandler.publish(TimiButton.Response());
      oled.printLine("--------------------", 4, 1);
      oled.printLine(TimiButton.Response().c_str(), 5, 1);
      delay(100);
      mqttHandler.publish(AnalogSensor.Response());
      oled.printLine(AnalogSensor.Response().c_str(), 6, 1);
      oled.drawProgressBar(60, 128, 4, int((100 * AnalogSensor.V) / AnalogSensor.Vmax));
      delay(100);
      LED.PowON(0);
      Buzzer.PowON(0);
    }
    if (message.indexOf("Reset") != -1) {
      ESP.reset();
    }
    if (message.indexOf("Silent") != -1) {
      Buzzer.PowON(0);
      LED.PowON(0);
      message = "";
      while (message.indexOf("Check") == -1) {
        mqttHandler.handleMqttLoop();
        if (message.indexOf("Check") != -1) { ESP.reset(); }
      }
    }
    delay(100);
  }
  message = "";
  if (TimiButton.Response().indexOf("alarmON") != -1) {
    for (int k = 0; k < 5; k++) {
      mqttHandler.publish(TimiButton.Response());
      LED.PowON(0);
      delay(100);
    }
    Buzzer.PowON(512);
    LED.PowON(512);
  }
  if (AnalogSensor.Response().indexOf("alarmON") != -1) {
    mqttHandler.publish(AnalogSensor.Response());
    Buzzer.PowON(512);
    delay(100);
  }
  delay(100);
  Buzzer.PowON(0);
  LED.PowON(0);
}
/*==========================================================================-*/
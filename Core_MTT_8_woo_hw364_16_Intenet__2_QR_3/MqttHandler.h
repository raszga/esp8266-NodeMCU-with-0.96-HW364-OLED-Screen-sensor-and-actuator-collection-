#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "SensorHandler.h"
// --- MQTT CONFIGURATION: DECLARATIONS ONLY (use extern) ---
extern const char* MQTT_SERVER;
extern const int MQTT_PORT;
extern const char* MQTT_USER;
extern const char* MQTT_PASSWORD;
extern const char* MQTT_TOPIC;
extern const char* MQTT_CLIENT_ID;
extern String message;

class MqttHandler {
  // ... rest of the class definition remains the same
public:
  MqttHandler();
  void begin(const char* ssid, const char* password);
  void handleMqttLoop();
  void publish(const String& payload);

private:
  WiFiClientSecure _espClient;
  PubSubClient _client;

  unsigned long _lastPublish = 0;
  const unsigned long PUBLISH_INTERVAL_MS = 10000;

  void _reconnect();
  static void _callback(char* topic, byte* payload, unsigned int length);
};

#endif  // MQTTHANDLER_H
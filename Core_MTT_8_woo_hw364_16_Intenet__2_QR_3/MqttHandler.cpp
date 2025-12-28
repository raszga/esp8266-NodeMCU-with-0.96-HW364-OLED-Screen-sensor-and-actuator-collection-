#include "MqttHandler.h"
#include "OLED_Utilities.h"
// --- MQTT CONFIGURATION: DEFINITIONS (Defined here once) yout HIVEMQ account ---
const char* MQTT_SERVER = "4ae0ff064ac54e2d9a978133ce707;k;k;lk;k18f.s1.eu.hivemq.cloud";
const int MQTT_PORT = 8883;
const char* MQTT_USER = "C22lk;kl;k081";
const char* MQTT_PASSWORD = "imkljisoakgggjhUHHNUIUra1900";
const char* MQTT_TOPIC = "Test";  // Lipova topic mama
const char* MQTT_CLIENT_ID = ESPid.c_str();
/*============================================================================*/
String message = "";
/*============================================================================*/
// --- Global instance pointer
MqttHandler* globalMqttHandlerInstance = nullptr;
/*============================================================================*/
MqttHandler::MqttHandler()
  : _client(_espClient) {
  globalMqttHandlerInstance = this;
}
/*============================================================================*/
/**
 * @brief Initializes Wi-Fi and MQTT client configuration.
 */
void MqttHandler::begin(const char* ssid, const char* password) {
  // 1. Start Wi-Fi connection
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // 2. Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // 3. Print connection details
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());  // Corrected to WiFi.SSID()
  // 4. Configure MQTT client
  _client.setServer(MQTT_SERVER, MQTT_PORT);
  _client.setCallback(_callback);
}
/*============================================================================*/
/**
 * @brief Handles MQTT reconnection logic (MQTTS + insecure).
 */
void MqttHandler::_reconnect() {
  // REQUIRED FOR HIVE MQ CLOUD: Skip certificate validation
  _espClient.setInsecure();

  while (!_client.connected()) {
    Serial.print("Attempting MQTT (MQTTS) connection...");
    oled.printLine(String(ESP.getChipId()).c_str(), 0, 1);
    oled.printLine("Traying  MQTT", 3, 1);
    if (_client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected. Subscribing to topic.");
      _client.subscribe(MQTT_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
/*============================================================================*/
/**
 * @brief Manages the MQTT connection, reconnection, and loop processing (formerly loop() body).
 */
void MqttHandler::handleMqttLoop() {
  // 1. Maintain Connection
  if (!_client.connected()) {
    _reconnect();
  }
  _client.loop();  // Process network traffic (RX/TX)
}
/*============================================================================*/
/**
 * @brief Publishes a message to the defined MQTT topic.
 */
void MqttHandler::publish(const String& payload) {
  if (_client.connected()) {
    //   Serial.print("Publishing: ");
    //   Serial.println(payload);
    _client.publish(MQTT_TOPIC, payload.c_str());
  }
}
/*============================================================================*/
/**
 * @brief Callback function executed when an MQTT message is received.
 */
void MqttHandler::_callback(char* topic, byte* payload, unsigned int length) {
  message = "";
  //Serial.print("MQTT Received [");
  //Serial.print(topic);
  //Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
}
/*============================================================================*/
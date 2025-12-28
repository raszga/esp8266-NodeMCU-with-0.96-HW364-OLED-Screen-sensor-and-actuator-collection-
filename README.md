/*==============================================================================
link to 3D models
https://grabcad.com/library/esp8266-with-0-96-oled-hw364-1

FILE: Core_MTT_8_woo_hw364_16_Intenet__2_QR_3.ino  
  AUTHOR: CR22081
  VERSION: 1.0

  PURPOSE:
      Central runtime for an ESP‑based IoT node coordinating:
        • MQTT command/telemetry pipeline
        • OLED display output (text, progress bars, QR codes)
        • Digital + analog sensor acquisition
        • PWM and non‑PWM actuators (LED, buzzers)
        • Soft “internet heartbeat” sensor using pulsar timing

  ARCHITECTURE:
      Hardware abstraction is fully delegated to dedicated modules:
          - OLED_Utilities.h      → display primitives + QR generator
          - MqttHandler.h         → WiFi connect, MQTT loop, RX/TX
          - SensorHandler.h       → struct‑based sensor model + callbacks
          - ActuatorManager.h     → unified actuator interface (PWM + digital)

      This file only orchestrates:
          - Initialization of all subsystems
          - Event‑driven MQTT command handling
          - Local alarm logic for button + analog thresholds
          - Periodic OLED updates and heartbeat publishing
          - Deterministic actuator resets per loop cycle

  RUNTIME BEHAVIOR:
      • Boot:
            - Init sensors/actuators
            - Init OLED (I2C pins 14/12)
            - Display ESP ID
            - Connect WiFi + MQTT
            - Emit startup buzzer pulse
            - Generate initial QR code

      • Loop:
            - Maintain MQTT loop
            - Parse inbound commands:
                  "cls"        → clear OLED
                  "Diff_ON"    → buzzer low PWM
                  "Diff_FULL"  → buzzer max PWM
                  "Diff_OFF"   → silence
                  "QRcode"     → generate QR
                  "alarmON"    → LED + buzzer alert + publish
                  "Check"      → publish sensor states + flash outputs
                  "Reset"      → ESP reset
                  "Silent"     → wait until next "Check"
            - Publish soft‑check failures (internet pulsar)
            - Update OLED uptime + sensor bars
            - Trigger local alarms from sensor responses
            - Enforce actuator OFF state at loop end

  NOTES:
      • Assumes global variables: message, ESPid, pulsarTime.
      • Designed for deterministic behavior and minimal hidden state.
      • Struct‑based config keeps hardware mapping explicit and maintainable.

## 🧩 Project Architecture
+-------------------------------------------------------------+
|                         Main Application                    |
|  Core_MTT_8_woo_hw364_16_Internet__2_QR_3.ino               |
+-------------------------+-----------------------------------+
|
v
+-------------------------------------------------------------+
|                       Sensor Subsystem                      |
|  SensorHandler.h / SensorHandler.cpp                        |
|  - Digital sensors                                          |
|  - Analog sensors (VCC-based)                               |
|  - Software sensors (internet pulse)                        |
|  - Unified API: Init, Activate, Check, Response             |
+-------------------------------------------------------------+
|
v
+-------------------------------------------------------------+
|                      Actuator Subsystem                     |
|  ActuatorManager.h / ActuatorManager.cpp                    |
|  - LED, Buzzer, Relay                                       |
|  - PWM or digital mode                                      |
|  - Ramping functions                                        |
+-------------------------------------------------------------+
|
v
+-------------------------------------------------------------+
|                        OLED Subsystem                       |
|  OLED_Utilities.h / OLED_Utilities.cpp                      |
|  - Text rendering                                           |
|  - Progress bar                                             |
|  - QR code generation                                       |
+-------------------------------------------------------------+
|
v
+-------------------------------------------------------------+
|                        MQTT Subsystem                       |
|  MqttHandler.h / MqttHandler.cpp                             |
|  - TLS-secured MQTT                                         |
|  - HiveMQ Cloud support                                     |
|  - Global message buffer                                    |
|  - Automatic reconnection                                   |
+-------------------------------------------------------------+

---

## 🛠️ Hardware Requirements

- **ESP8266 NodeMCU**  
- **0.96" OLED HW364** (SSD1306, I2C, 128×64)  
- **Button sensor** (GPIO 15)  
- **Analog water sensor** (reads VCC via ADC_MODE)  
- **Buzzer** (GPIO 5)  
- **Secondary buzzer** (GPIO 4)  
- **LED** (GPIO 2)  
- Optional: **Relay**
- 
### **I2C Pins (OLED)**
- SDA → GPIO 14  
- SCL → GPIO 12
- 
## 📦 Folder Structure

├── ActuatorManager.cpp
├── ActuatorManager.h
├── Core_MTT_8_woo_hw364_16_Intenet__2_QR_3.ino
├── MqttHandler.cpp
├── MqttHandler.h
├── OLED_Utilities.cpp
├── OLED_Utilities.h
├── SensorHandler.cpp
└── SensorHandler.h


---

## 🔧 Installation

### **1. Install Required Libraries**
- Adafruit GFX  
- Adafruit SSD1306  
- qrcode library  
- PubSubClient  
- ESP8266WiFi  
- WiFiClientSecure  

### **2. Configure WiFi**
In the `.ino` file:

```cpp
const char* MY_WIFI_SSID = "your_ssid";
const char* MY_WIFI_PASSWORD = "your_password";




  LAST UPDATED: <insert date>
==============================================================================*/


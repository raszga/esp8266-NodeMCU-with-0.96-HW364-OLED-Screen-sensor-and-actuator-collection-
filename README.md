/*==============================================================================
link to 3D models:https://grabcad.com/library/esp8266-with-0-96-oled-hw364-1
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

  LAST UPDATED: <insert date>
==============================================================================*/

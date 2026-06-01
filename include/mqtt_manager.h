#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

class mqtt_manager
{
  PubSubClient mqttClient;
  String deviceName;
  String deviceId;
  String mainTopic;

public:
  mqtt_manager(WiFiClient &espClient);
  void begin(const String &device_name, const String &deviceID, const String &deviceTopic);
  void connectMQTT();

  void publishConfig(
      const String &sensorType,
      const String &valueTemplate,
      const String &unitOfMeasurement,
      const String &deviceClass);

  void publishData(const std::vector<std::pair<String, float>> &measurements);
  bool isConnected() { return mqttClient.connected(); }
  bool loop() { return mqttClient.loop(); }
};

#endif // MQTT_MANAGER_H
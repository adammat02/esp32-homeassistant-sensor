#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

struct device
{
  String device_id;
  String name;
  String model;
  String manufacturer;
};

struct sensor
{
  String name;
  String unique_id;
  String state_topic;
  String unit_of_measurement;
  String device_class;
  String state_class;
  device dev;
};

class mqtt_manager
{
  PubSubClient mqttClient;
  device device_;

  JsonDocument struct_to_json(const sensor &sensor);

public:
  mqtt_manager(WiFiClient &espClient);
  void begin(const device &device);

  void connect_mqtt();

  void publish_config(const sensor &sensor);

  void publish_data(const std::vector<std::pair<sensor, float>> &measurements);
  bool is_connected() { return mqttClient.connected(); }
  bool loop() { return mqttClient.loop(); }
};

#endif // MQTT_MANAGER_H
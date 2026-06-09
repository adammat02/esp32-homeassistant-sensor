#include "mqtt_manager.h"
#include "secrets.h"

mqtt_manager::mqtt_manager(WiFiClient& espClient) : mqttClient(espClient) {}

JsonDocument mqtt_manager::struct_to_json(const sensor &sensor)
{
  JsonDocument doc;
  doc["name"]                = sensor.name;
  doc["unique_id"]           = sensor.unique_id;
  doc["state_topic"]         = sensor.state_topic;
  doc["value_template"]      = "{{ value_json." + String(sensor.device_class) +  "}}";
  doc["unit_of_measurement"] = sensor.unit_of_measurement;
  doc["device_class"]        = sensor.device_class;
  doc["state_class"]         = sensor.state_class;

  JsonObject device = doc["device"].to<JsonObject>();
  device["name"]         = sensor.dev.name;
  device["model"]        = sensor.dev.model;
  device["manufacturer"] = sensor.dev.manufacturer;

  JsonArray identifiers = device["identifiers"].to<JsonArray>();
  identifiers.add(sensor.dev.device_id);

  return doc;
}

void mqtt_manager::begin(const device &device)
{
  device_ = device;
  mqttClient.setBufferSize(512);
} 

void mqtt_manager::connect_mqtt()
{
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  while (!mqttClient.connected())
  {
    Serial.print("Laczenie z MQTT...");
    if (mqttClient.connect(device_.device_id.c_str(), MQTT_LOGIN, MQTT_PASSWD))
    {
      Serial.println("OK");
    }
    else
    {
      Serial.print("blad, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" - ponowna proba za 2 s");
      delay(2000);
    }
  }
}

void mqtt_manager::publish_config(const sensor &sensor)
{
  JsonDocument doc = struct_to_json(sensor);
  char payload[512];
  serializeJson(doc, payload);

  String configTopic = "homeassistant/sensor/" + sensor.unique_id + "/config";
  mqttClient.publish(configTopic.c_str(), payload, true);

  Serial.print("Publikacja na topic: ");
  Serial.println(configTopic);
  Serial.println(payload);
}

void mqtt_manager::publish_data(const std::vector<std::pair<sensor, float>> &measurements)
{
  JsonDocument doc;
  for (const auto &m : measurements) {
      doc[m.first.device_class] = m.second;
  }

  char payload[256];
  serializeJson(doc, payload);
  
  String topic = measurements[0].first.state_topic;
  mqttClient.publish(topic.c_str(), payload);

  Serial.print("Publikacja na topic: ");
  Serial.println(topic);
  Serial.println(payload);
}

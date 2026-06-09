#include "mqtt_manager.h"
#include "secrets.h"

mqtt_manager::mqtt_manager(WiFiClient& espClient) : mqttClient(espClient) {}

void mqtt_manager::begin(const String &device_name,const String& deviceID, const String& deviceTopic)
{
  deviceName = device_name;
  mainTopic = deviceTopic;
  deviceId = deviceID;
  mqttClient.setBufferSize(512);
} 

void mqtt_manager::connectMQTT()
{
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  while (!mqttClient.connected())
  {
    Serial.print("Laczenie z MQTT...");
    if (mqttClient.connect(deviceId.c_str(), MQTT_LOGIN, MQTT_PASSWD))
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

void mqtt_manager::publishConfig(
    const String &sensorType,
    const String &valueTemplate,
    const String &unitOfMeasurement,
    const String &deviceClass)
{
    JsonDocument doc;
    doc["name"]                = deviceName + " " + sensorType;
    doc["unique_id"]           = deviceId + "_" + sensorType;
    doc["state_topic"]         = mainTopic;
    doc["value_template"]      = valueTemplate;
    doc["unit_of_measurement"] = unitOfMeasurement;
    doc["device_class"]        = deviceClass;
    doc["state_class"]         = "measurement";

    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]         = deviceName;
    device["model"]        = "ESP32";
    device["manufacturer"] = "Espressif";

    JsonArray identifiers = device["identifiers"].to<JsonArray>();
    identifiers.add(deviceId);

    char payload[512];
    serializeJson(doc, payload);

    String configTopic = "homeassistant/sensor/" + deviceId + "/" + sensorType + "/config";
    mqttClient.publish(configTopic.c_str(), payload, true);

    Serial.print("Publikacja na topic: ");
    Serial.println(configTopic);
    Serial.println(payload);
}

void mqtt_manager::publishData(const std::vector<std::pair<String, float>> &measurements)
{
    JsonDocument doc;
    for (const auto &m : measurements) {
        doc[m.first] = m.second;
    }

    char payload[256];
    serializeJson(doc, payload);

    mqttClient.publish(mainTopic.c_str(), payload);

    Serial.print("Publikacja na topic: ");
    Serial.println(mainTopic);
    Serial.println(payload);
}

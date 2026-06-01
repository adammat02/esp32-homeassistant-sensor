#include <Arduino.h>
#include "wifi_manager.h"
#include "mqtt_manager.h"

WiFiClient espClient;
mqtt_manager mqttClient(espClient);

String deviceName;
String deviceId;
String topic;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  deviceName = "ESP32";
  deviceId = "esp32";
  topic = "home/" + deviceId + "/state";

  mqttClient.begin(deviceName, deviceId, topic);

  connectWiFi();
  mqttClient.connectMQTT();

  mqttClient.publishConfig("temperature", "{{ value_json.temperature }}", "°C", "temperature");
  mqttClient.publishConfig("humidity", "{{ value_json.humidity }}", "%", "humidity");
}

void loop()
{
  if (!mqttClient.isConnected())
  {
    mqttClient.connectMQTT();
  }
  mqttClient.loop();

  mqttClient.publishData({{"temperature", 23.4}, {"humidity", 61.2}});
  delay(5000);
}

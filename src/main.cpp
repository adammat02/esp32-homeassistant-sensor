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
  delay(2000);

  mqttClient.publishConfig("temperature", "{{ value_json.temperature }}", "°C", "temperature");
  mqttClient.publishConfig("humidity", "{{ value_json.humidity }}", "%", "humidity");
}

void loop()
{
  int temp = 0;
  while(true)
  {
    if (!mqttClient.isConnected())
    {
      mqttClient.connectMQTT();
    }
    mqttClient.loop();

    mqttClient.publishData({{"temperature", temp}, {"humidity", 61.2}});
    temp = (temp + 5) % 25;
    delay(5000);
  }
}

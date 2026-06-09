#include <Arduino.h>
#include "secrets.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"

WiFiClient espClient;
mqtt_manager mqttClient(espClient);

device dev = {
  .device_id = "esp32_01",
  .name = "ESP32",
  .model = "ESP32-WROOM",
  .manufacturer = "Espressif"
};

sensor temp_sensor = {
  .name = "Temperatura",
  .unique_id = "esp32_01_temp",
  .state_topic = "home/esp32_01/state",
  .unit_of_measurement = "°C",
  .device_class = "temperature",
  .state_class = "measurement",
  .dev = dev
};

sensor hum_sensor = {
  .name = "Wilgotność",
  .unique_id = "esp32_01_hum",
  .state_topic = "home/esp32_01/state",
  .unit_of_measurement = "%",
  .device_class = "humidity",
  .state_class = "measurement",
  .dev = dev
};

void setup()
{
  Serial.begin(115200);
  delay(1000);

  mqttClient.begin(dev);

  connect_wifi(WIFI_SSID, WIFI_PASSWD);
  mqttClient.connect_mqtt();
  delay(2000);

  mqttClient.publish_config(temp_sensor);
  mqttClient.publish_config(hum_sensor);
}

void loop()
{
  int temp = 0;
  while(true)
  {
    if (!mqttClient.is_connected())
    {
      mqttClient.connect_mqtt();
    }
    mqttClient.loop();

    mqttClient.publish_data({{temp_sensor, temp}, {hum_sensor, 61.2}});
    temp = (temp + 5) % 25;
    delay(5000);
  }
}

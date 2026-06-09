#include <Arduino.h>
#include "secrets.h"
#include "wifi_manager.h"
#include "mqtt_manager.h"
#include <DHT.h>

#define DHT11_PIN 5
#define DHTTYPE DHT11

WiFiClient espClient;
mqtt_manager mqttClient(espClient);

DHT dht(DHT11_PIN, DHTTYPE);

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

RTC_DATA_ATTR int bootCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(500);

  mqttClient.begin(dev);

  connect_wifi(WIFI_SSID, WIFI_PASSWD);
  mqttClient.connect_mqtt();
  dht.begin();
  delay(500);

  esp_sleep_enable_timer_wakeup(60000000);

  if (bootCount == 0)
  {
    mqttClient.publish_config(temp_sensor);
    mqttClient.publish_config(hum_sensor); 
  }
  bootCount++;

  if (!mqttClient.is_connected())
  {
    mqttClient.connect_mqtt();
  }
  mqttClient.loop();

  float hum = dht.readHumidity();
  float temp = dht.readTemperature();

  mqttClient.publish_data({{temp_sensor, temp}, {hum_sensor, hum}});

  esp_deep_sleep_start();
}

void loop()
{

}

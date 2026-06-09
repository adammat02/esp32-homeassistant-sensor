#include "wifi_manager.h"

#define MAX_ATTEMPTS 10

bool connect_wifi(const String &wifi_ssid, const String &wifi_passwd)
{
  Serial.print("Wi-Fi connecting: ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_passwd);

  int connect_attempts = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    connect_attempts++;
    if (connect_attempts > MAX_ATTEMPTS)
    {
      Serial.println("Connection limit exceeded");
      return false;
    }
  }

  Serial.println();
  Serial.println("Wi-Fi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  return true;
}


bool is_wifi_connected()
{
  return WiFi.status() == WL_CONNECTED;
}
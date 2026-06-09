#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <string>
#include <WiFi.h>

bool connect_wifi(const String &wifi_ssid, const String &wifi_passwd);
bool is_wifi_connected();

#endif // WIFI_MANAGER_H
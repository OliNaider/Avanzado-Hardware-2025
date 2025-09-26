#include <WiFi.h>
#include "esp_wifi.h"   // para esp_wifi_get_mac

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Inicializar WiFi en modo estación
  WiFi.mode(WIFI_STA);

  // --- Método 1: usando WiFi.macAddress() ---
  String macStr = WiFi.macAddress();
  Serial.print("MAC (string): ");
  Serial.println(macStr);

  // --- Método 2: usando esp_wifi_get_mac() ---
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac); // WIFI_IF_STA = interfaz estación

  Serial.print("// Dirección MAC del receptor (ESP32)\n");
  Serial.print("uint8_t broadcastAddress[] = {");
  for (int i = 0; i < 6; i++) {
    Serial.print("0x");
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(", ");
  }
  Serial.println("};");
}

void loop() {}

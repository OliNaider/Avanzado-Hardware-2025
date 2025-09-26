#include <WiFi.h>
#include <DHT.h>
#include <esp_now.h>

// Configuración del DHT
#define DHTPIN 4  // Pin donde está conectado el sensor
#define DHTTYPE DHT11  // Cambia a DHT11 si estás usando un DHT11

DHT dht(DHTPIN, DHTTYPE);

// Configuración WiFi
const char* ssid = "IoTB";
const char* password = "inventaronelVAR";

// Dirección MAC del receptor (ESP32-CAM)
uint8_t broadcastAddress[] = {0xA0, 0xA3, 0xB3, 0x28, 0xA6, 0xD8};

// Estructura del mensaje
typedef struct estructura {
  char msg[32];
} estructura;
estructura myData;


void mensaje1();

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);
  
  esp_now_init();
  registrarPeer();
  Serial.println(myData.msg);

}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  mensaje1(h,t);
}


void registrarPeer() {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void mensaje1(float h, float t) {
 
  snprintf(myData.msg, sizeof(myData.msg), "H:%.1f T:%.1f", h, t);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

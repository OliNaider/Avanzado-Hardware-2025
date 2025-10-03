#include <WiFi.h>
#include <DHT.h>
#include <esp_now.h>

// Configuración del DHT
#define DHTPIN 4  
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);

// Estructura para enviar mensajes
typedef struct estructura {
  char msg[32];
} estructura;

estructura myData;
// Estructura para recibir mensajes
estructura incomingData;

String datosRecibidos = "";

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  Serial.print("Mensaje recibido: ");
  Serial.println(incomingData.msg);
  datosRecibidos = String(incomingData.msg);
}

uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0xE7, 0x60, 0xBC}; // MAC del otro ESP32

#define pinRele 32

void registrarPeer() {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al registrar peer");
  }
}

void mensaje1(float h, float t) {
  snprintf(myData.msg, sizeof(myData.msg), "H:%.1f T:%.1f", h, t);
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  
  registrarPeer();

  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "sensor") {
      float h = dht.readHumidity();
      float t = dht.readTemperature();

      if (isnan(h) || isnan(t)) {
        Serial.println("Error leyendo sensor DHT");
      } else {
        Serial.printf("Humedad: %.1f %%  Temperatura: %.1f °C\n", h, t);
        mensaje1(h, t);
      }
    }
  }

  if (datosRecibidos == "Botón presionado") {
    digitalWrite(pinRele, HIGH);
    delay(3000);
    digitalWrite(pinRele, LOW);
    Serial.println("Rele encendido por botón");
    datosRecibidos = ""; // Limpio para evitar repetición
  }
}

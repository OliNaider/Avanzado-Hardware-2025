#include <WiFi.h>
#include <esp_now.h>

// Estructura para enviar mensajes
typedef struct estructura {
  char msg[32];
} estructura;

estructura myData;
estructura incomingData;
String datosRecibidos = "";

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataBytes, int len) {
  memcpy(&incomingData, incomingDataBytes, sizeof(incomingData));
  Serial.print("Mensaje recibido: ");
  Serial.println(incomingData.msg);
  datosRecibidos = String(incomingData.msg);
}

uint8_t broadcastAddress[] = {0xA0, 0xA3, 0xB3, 0x28, 0xA6, 0xD8}; // MAC del ESP32 con sensor + relé

const int botonPin = 15;  


void registrarPeer() {
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Error al registrar peer");
  }
}

void mensajeBotonPresionado() {
  snprintf(myData.msg, sizeof(myData.msg), "Botón presionado");
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  registrarPeer();
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(botonPin, INPUT_PULLUP);
}

void loop() {
  int lectura = digitalRead(botonPin);


  if (lectura == LOW) {
    // Botón fue presionado
    Serial.println("Botón presionado");
    mensajeBotonPresionado();
  }

  delay(500);
 
}


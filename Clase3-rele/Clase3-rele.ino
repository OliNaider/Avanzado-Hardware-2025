#include <WiFi.h>
#include <WebServer.h>


#define pinRele 4
const char* ssid = "IoTB"; // Reemplaza con el nombre de tu red WiFi
const char* password = "inventaronelVAR";      // Reemplaza con tu contraseña


WebServer server(80);
bool releState = LOW; 

void handleRoot();
void handleOn();
void handleOff();

void setup() {
  pinMode(pinRele, OUTPUT);
  digitalWrite(pinRele, releState); // Asegurarse que el relé inicie apagado

  Serial.begin(115200);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Rutas del servidor web
  server.on("/", handleRoot);   // Página principal
  server.on("/on", handleOn);   // Acción para encender
  server.on("/off", handleOff); // Acción para apagar

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 Control Rele</title>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

  html += "<body>";
  html += "<h1>Control de Rele con ESP32</h1>";
  
  // Agrega los botones
  html += "<p>";
  html += "<a href='/on' class='button' style='background-color:#28a745;'>ENCENDER</a>";
  html += "<a href='/off' class='button' style='background-color:#dc3545;'>APAGAR</a>";
  html += "</p>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}


void handleOn() {
  releState = HIGH;
  digitalWrite(pinRele, releState);
  Serial.println("Relé encendido.");
  server.sendHeader("Location", "/"); // Redirecciona al cliente a la página principal
  server.send(302, "text/plain", "Encendiendo...");
}

void handleOff() {
  releState = LOW;
  digitalWrite(pinRele, releState);
  Serial.println("Relé apagado.");
  server.sendHeader("Location", "/"); // Redirecciona al cliente a la página principal
  server.send(302, "text/plain", "Apagando...");
}

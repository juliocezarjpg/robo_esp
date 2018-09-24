#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

//upload_port = espota.local

ESP8266WebServer server(80);

char ssid[] = "Samsung";
char password[] = "juliogatao";

void connect_wifi();
void frente();
void tras();
void direita();
void esquerda();
void parar();
void wifistatus();

void setup() {
  Serial.begin(115200);
  // WiFi.mode(WIFI_STA);

  pinMode(LED_BUILTIN, OUTPUT);

  //Pinos dos motores
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(LED_BUILTIN,1);

  digitalWrite(D1, 0);
  digitalWrite(D3, 0);
  digitalWrite(D2, 0);
  digitalWrite(D4, 0);

  connect_wifi();
  server.on("/frente", frente);
  server.on("/tras", tras);
  server.on("/direita", direita);
  server.on("/esquerda", esquerda);
  server.on("/parar", parar);
  server.on("/wifistatus", wifistatus);
  server.begin();

  ArduinoOTA.setHostname("espota");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) connect_wifi();
}

void connect_wifi() {
  WiFi.begin(ssid, password);
  WiFi.hostname("espota");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN,1);
    delay(500);
    digitalWrite(LED_BUILTIN,0);
    delay(500);
  }
}

void wifistatus() {
  if (WiFi.status() == WL_CONNECTED)
    server.send(200, "text/plain", "1");
  else
    server.send(200, "text/plain", "0");
}

void frente() {
  digitalWrite(D1, 1);
  digitalWrite(D3, 0);
  digitalWrite(D2, 1);
  digitalWrite(D4, 0);
}

void parar(){
  digitalWrite(D1, 0);
  digitalWrite(D3, 0);
  digitalWrite(D2, 0);
  digitalWrite(D4, 0);
}

void tras(){
  digitalWrite(D1, 1);
  digitalWrite(D3, 1);
  digitalWrite(D2, 1);
  digitalWrite(D4, 1);
}

void direita(){
  digitalWrite(D1, 1);
  digitalWrite(D3, 1);
  digitalWrite(D2, 1);
  digitalWrite(D4, 0);
}

void esquerda(){
  digitalWrite(D1, 1);
  digitalWrite(D3, 0);
  digitalWrite(D2, 1);
  digitalWrite(D4, 1);
}

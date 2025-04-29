#include <WiFi.h>
#include <WebSocketsServer.h>
#include <DHT.h>
#include <SPI.h>
#include <MFRC522.h>
#include "SPIFFS.h"
#include <time.h>

// Definições de pinos
#define DHTPIN 2         // Pino do DHT11
#define DHTTYPE DHT11
#define MQ9_PIN 35       // Pino analógico do MQ-9 (entrada de gás)
#define SS_PIN 4         // Pino Slave Select do RFID
#define RST_PIN 22       // Pino Reset do RFID

// Instâncias dos sensores e do leitor RFID
DHT dht(DHTPIN, DHTTYPE);
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Credenciais do Wi-Fi
const char* ssid = "Nome da rede";
const char* password = "Senha da rede";

// Inicialização dos servidores WebSocket e HTTP
WebSocketsServer webSocket = WebSocketsServer(81);
WiFiServer httpServer = WiFiServer(80);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.printf("Mensagem recebida de [%u]: %s\n", num, msg.c_str());

    if (msg == "simularUmidade") {
      String json = "{\"temperature\":25,\"humidity\":95,\"gas\":500}";
      webSocket.broadcastTXT(json);
      Serial.println("Simulação de alta umidade enviada.");
    }
    else if (msg == "simularTempAlta") {
      String json = "{\"temperature\":32,\"humidity\":50,\"gas\":500}";
      webSocket.broadcastTXT(json);
      Serial.println("Simulação de temperatura alta enviada.");
    }
    else if (msg == "simularTempBaixa") {
      String json = "{\"temperature\":15,\"humidity\":50,\"gas\":500}";
      webSocket.broadcastTXT(json);
      Serial.println("Simulação de temperatura baixa enviada.");
    }
    else if (msg == "simularGasAlto"){
      String json = "{\"temperature\":20,\"humidity\":50,\"gas\":2000}";
      webSocket.broadcastTXT(json);
      Serial.println("Simulação de Gás alto enviada.");

    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando sensores...");

  dht.begin();
  pinMode(MQ9_PIN, INPUT);
  SPI.begin();
  mfrc522.PCD_Init();

  if (!SPIFFS.begin(true)) {
    Serial.println("Erro ao montar SPIFFS!");
  } else {
    Serial.println("SPIFFS montado com sucesso.");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi. IP: " + WiFi.localIP().toString());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("Horário sincronizado.");
  } else {
    Serial.println("Falha ao sincronizar horário.");
  }

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  httpServer.begin();
}

void loop() {
  webSocket.loop();

  WiFiClient client = httpServer.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print("<html><body><h1>ESP32 WebSocket Server</h1></body></html>");
    delay(1);
    client.stop();
  }

  static unsigned long lastSensorTime = 0;
  if (millis() - lastSensorTime >= 5000) {
    lastSensorTime = millis();

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    int nivelGas = analogRead(MQ9_PIN);

    String json = "{";
    bool added = false;

    if (!isnan(temperatura)) {
      json += "\"temperature\":" + String(temperatura);
      added = true;
    }
    if (!isnan(umidade)) {
      if (added) json += ",";
      json += "\"humidity\":" + String(umidade);
      added = true;
    }
    if (nivelGas >= 0) {
      if (added) json += ",";
      json += "\"gas\":" + String(nivelGas);
    }
    json += "}";

    webSocket.broadcastTXT(json);
    Serial.println("Dados enviados: " + json);
  }

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidStr = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uidStr += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidStr.toUpperCase();

    bool jaRegistrado = false;
    File uidFile = SPIFFS.open("/registered_uids.txt", FILE_READ);
    if (uidFile) {
      while (uidFile.available()) {
        String linha = uidFile.readStringUntil('\n');
        linha.trim();
        if (linha == uidStr) {
          jaRegistrado = true;
          break;
        }
      }
      uidFile.close();
    }

    if (!jaRegistrado) {
      File writeFile = SPIFFS.open("/registered_uids.txt", FILE_APPEND);
      if (writeFile) {
        writeFile.println(uidStr);
        writeFile.close();
      }
    }

    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char timeStr[64];
      strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeinfo);
      String logEntry = String(timeStr) + " - Cartão lido: " + uidStr;

      File logFile = SPIFFS.open("/access_log.txt", FILE_APPEND);
      if (logFile) {
        logFile.println(logEntry);
        logFile.close();
      }

      String rfidJson = "{\"rfid\":\"" + uidStr + "\",\"timestamp\":\"" + String(timeStr) + "\"}";
      webSocket.broadcastTXT(rfidJson);
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    delay(1000);
  }

  delay(10);
}

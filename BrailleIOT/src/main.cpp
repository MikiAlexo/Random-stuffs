#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

const char* ssid = "ESP8266_Hotspot";
const char* password = "12345678";

ESP8266WebServer server(80);
WiFiServer tcpServer(5000);
WiFiClient tcpClient;

void handleRoot() {
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "File not found");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

void handleSend() {
  if (server.hasArg("data")) {
    String msg = server.arg("data");
    Serial.println("Received: " + msg);

    // If Processing is connected, send it too
    if (tcpClient && tcpClient.connected()) {
      tcpClient.println(msg);
    }

    server.send(200, "text/plain", "Message received: " + msg);
  } else {
    server.send(400, "text/plain", "No data received");
  }
}

void setup() {
  Serial.begin(115200);
  LittleFS.begin();

  WiFi.softAP(ssid, password);
  Serial.print("Hotspot started. IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/send", handleSend);
  server.begin();

  tcpServer.begin();
  Serial.println("TCP server started on port 5000");
}

void loop() {
  server.handleClient();

  // accept client connection
  if (!tcpClient || !tcpClient.connected()) {
    tcpClient = tcpServer.available();
    if (tcpClient) {
      Serial.println("Processing client connected");
    }
  }
}

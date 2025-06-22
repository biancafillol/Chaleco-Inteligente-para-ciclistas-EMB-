#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "MovistarFibraBOVER";
const char* password = "carlosbover2008";
const char* serverIP = "192.168.1.79";
const uint16_t serverPort = 8080;

WiFiClient client;
const int botonPin18 = 18;
const int botonPin17 = 17;
const int botonPin16 = 16;
const int buttonPin4 = 4;

void setup() {
  pinMode(botonPin18, INPUT_PULLUP);
  pinMode(botonPin17, INPUT_PULLUP);
  pinMode(botonPin16, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (digitalRead(buttonPin4) == LOW) {
    if (client.connect(serverIP, serverPort)) {
      client.print("A");
      client.stop();
    }
    delay(200);
  }
  if (digitalRead(botonPin18) == LOW) {
    if (client.connect(serverIP, serverPort)) {
      client.print("B");
      client.stop();
    }
    delay(200);
  }
  if (digitalRead(botonPin17) == LOW) {
    if (client.connect(serverIP, serverPort)) {
      client.print("C");
      client.stop();
    }
    delay(200);
  }
    if (digitalRead(botonPin16) == LOW) {
    if (client.connect(serverIP, serverPort)) {
      client.print("D");
      client.stop();
    }
    delay(200);
  }
}

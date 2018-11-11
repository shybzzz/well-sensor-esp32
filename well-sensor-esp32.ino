#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "definitions.h"

void serialStr(char* str) {
  for(int i = 0; i < strlen(str); i++) {
    Serial.print(char(str[i]));
  }
  Serial.println();
}

#include "init-pins.h"
#include "init-qr.h"
#include "init-spiffs.h"
#include "init-wifi.h"
#include "init-reset-button.h"
#include "parse.h"
#include "sockets.h"
#include "listen-ap.h"
#include "init-mqtt.h"

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Well Sensor...");

  if(!SPIFFS.begin()){
    Serial.println("Failed to Mount File System");
    return;
  }

  initPins();
  initResetButton();
  initQr();
  initWifi();  
  initMqtt();
  Serial.println("Well Sensor is running");
}

void loop() {

  if(buttonClicked) {
    isWifiConfigSet = !isWifiConfigSet;
    buttonClicked = false;
    restartWifi();
    return;
  }

  WiFiClient client = wifiServer.available();

  if(!isWifiConfigSet) {
    redLight();
    listenSetWifiConfig(client);
    listenGetIp(client);
    return;    
  }

  if(!isWifiConnected()) {
    yellowLight();
    reconnectWifi();
    return;
  }

  noLight();

  delay(750);
  client.stop();

  if(!isMqttConfigSet) {
    greenLight();
    return;
  }

  if(!mqttClient.connected()) {
    blueLight();
    connectMqtt();
    return;
  }
  
  mqttClient.loop();
  
  dance(100);
  
  
  
}

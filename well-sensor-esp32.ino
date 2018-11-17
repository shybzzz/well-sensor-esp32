#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"

#include "definitions.h"

#include "init-pins.h"
#include "init-qr.h"
#include "init-spiffs.h"
#include "init-wifi.h"
#include "init-reset-button.h"
#include "init-sockets.h"
#include "init-data.h"
#include "init-mqtt.h"

#include "data-median.h"
#include "filter-median.h"

#include "routine-listen-ap.h"
#include "routine-data.h"

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
    listenGetWifiConfig(client);
    return;    
  }

  if(!isWifiConnected()) {
    yellowLight();
    reconnectWifi();
    return;
  }

  delay(750);
  client.stop();

  if(!isMqttConfigSet) {
    greenLight();
    return;
  }

  if(!mqttClient.connected()) {
    blueLight();
    reconnectMqtt();
    return;
  }

  noLight();

  if(runDataRoutine()){
    dance(100);
  }
 
  mqttClient.loop();  
}

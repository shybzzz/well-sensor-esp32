#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"

#include "definitions.h"

#include "init-led.h"
#include "init-button.h"
#include "init-qr.h"
#include "init-spiffs.h"
#include "init-server.h"
#include "init-data.h"
#include "init-wifi.h"
#include "init-mqtt.h"
#include "init-adc.h"

#include "data-median.h"
#include "data-adc.h"

#include "filter-median.h"
#include "filter-exp-smooth.h"
#include "filter-mean.h"

#include "routine-listen-ap.h"
#include "routine-data.h"

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Well Sensor...");

  initLed();
  initButton();
  initQr();

  if(initSPIFSS()) {
    initWifi();  
    initMqtt();
  }
    
  Serial.println("Well Sensor is running");
}

void loop() {

  if(buttonClicked) {
    isAPRunning
      ? stopAP()
      : startAP();
    buttonClicked = false;    
    return;
  }

  listenServer();

  if(isAPRunning) {
    whiteLight();
    return;
  }

  if(!isWifiConfigSet) {
    redLight();    
    return;    
  }

  if(!isWifiConnected()) {
    yellowLight();
    reconnectWifi();
    return;
  }

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

  if(runDataRoutine(getADC_Data)){
    dance(100);
  }
 
  mqttClient.loop();  
}

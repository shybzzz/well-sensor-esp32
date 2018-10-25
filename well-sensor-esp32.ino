#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"

#include "init-pins.h"
#include "init-reset-button.h"
#include "init-qr.h"
#include "init-wifi.h"



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

  Serial.println("Well Sensor is running");
}

void loop() {

  if(buttonClicked) {
    formatFlash();
  }

  WiFiClient client = wifiServer.available();

  if(!isWifiConfigSet) {
    
    redLight();
    
    char* wifiData = readSocket(client, 1);
    if(wifiData) {
      Serial.println("Wifi Data");
      Serial.println(wifiData);
    } else {
      return;
    }
    
  }
  
  dance(1000);
  
}

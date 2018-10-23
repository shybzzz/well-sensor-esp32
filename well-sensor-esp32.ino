#include "Arduino.h"
#include "init-pins.h"
#include "init-reset-button.h"

#include <WiFi.h>
WiFiServer wifiServer(80);

#include "SPIFFS.h"

void formatFlash(){
  Serial.println("Removing Stored Config...");
  SPIFFS.format();
  Serial.println("Restarting Well Sensor...");
  delay(500);
  ESP.restart();
  
}

struct QRConfig {
  char AP_SSID[64];
  char AP_PWD[64];
};

QRConfig qrConfig;

#include <ArduinoJson.h>     

struct WifiConfig {
  char ssid[64];
  char pwd[64];
};

const char *wifiName = "/wifiConfig.json";
WifiConfig wifiConfig;
bool isWifiConfigSet = false;

void loadWifiConfig(){
  if(!SPIFFS.exists(wifiName)){
    Serial.print(wifiName);
    Serial.println(" does not exist");
    return;
  }

  File wifiFile = SPIFFS.open(wifiName, "r");
  if (wifiFile) 
  {
    size_t size = wifiFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    wifiFile.readBytes(buf.get(), size);
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());
    if (json.success()) 
    {
      Serial.println("Wifi Configuration");
      json.prettyPrintTo(Serial);
      strlcpy(wifiConfig.ssid, json["ssid"], sizeof(wifiConfig.ssid));
      strlcpy(wifiConfig.pwd, json["pwd"], sizeof(wifiConfig.pwd));
      isWifiConfigSet = true;      
    } else {
      Serial.println("Wifi Configuration is corrupted");
    }
    wifiFile.close();
  } else {
      Serial.println("Failed opening Wifi Configuration File");
  }
}

void setup() {

  Serial.begin(115200);
  Serial.println("Starting Well Sensor...");

  initPins();
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleIntr, HIGH);

  strlcpy(qrConfig.AP_SSID, "Well Sensor", sizeof(qrConfig.AP_SSID));
  strlcpy(qrConfig.AP_PWD, "11111111", sizeof(qrConfig.AP_PWD));
  
  
  wifiServer.begin();
  Serial.println("Wifi Server is sarted");
  
  if(!SPIFFS.begin()){
    Serial.println("Failed to Mount File System");
    return;
  }

  loadWifiConfig();
  if(isWifiConfigSet){
    Serial.println("Connecting Wifi");
    if(WiFi.begin(wifiConfig.ssid, wifiConfig.pwd) != WL_CONNECTED){
      Serial.println("Wifi is not connected");
    }
  } else {
    Serial.println("Starting Access Point");
    WiFi.softAP(qrConfig.AP_SSID, qrConfig.AP_PWD);
  }

  Serial.println("Well Sensor is running");
}



void loop() {

  if(buttonClicked) {
    formatFlash();
  }
  
  dance(1000);
  
}

#ifndef __INIT_WIFI__
#define __INIT_WIFI__

WiFiServer wifiServer(80);

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
    if (json.success()) {
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

void initWifi() {
  
    loadWifiConfig();
  if(isWifiConfigSet){
    Serial.println("Connecting Wifi");
    if(WiFi.begin(wifiConfig.ssid, wifiConfig.pwd) != WL_CONNECTED){
      Serial.println("Wifi is not connected");
    }
  } else {
    Serial.println("Starting Access Point");
    WiFi.softAP(qrConfig.AP_SSID, qrConfig.AP_PWD);
    Serial.println(WiFi.softAPIP());
  }

  wifiServer.begin();
  Serial.println("Wifi Server is started");  
  
}

char* readSocket(WiFiClient& client, uint8_t dataType) {
  
  char* buff = nullptr;
  if(client && client.connected()) {
    Serial.println("Client Connected");
    Serial.print("Message Length: ");
    Serial.println(client.available());
    if(client.available()) {
      byte msgType = client.read();
      if(msgType == dataType) {
        Serial.print("Data Type: ");
        Serial.println(dataType);
        uint16_t msgLength = client.available();
        Serial.print("Message Length: ");
        Serial.println(msgLength);
        buff = new char[msgLength];
        client.read(reinterpret_cast<uint8_t*>(buff), msgLength);
      } else {
        Serial.print("Not supported message type: ");
        Serial.println(msgType);
        client.write(dataType);
      }
    } else {
      client.write(dataType);
    }
    delay(500);
    client.stop();      
  }
  
  return buff;  
}

#endif

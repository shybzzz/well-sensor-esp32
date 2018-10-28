#ifndef __INIT_WIFI__
#define __INIT_WIFI__

WiFiServer wifiServer(80);

struct WifiConfig {
  char ssid[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
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
      strlcpy(wifiConfig.ssid, json["ssid"], MAX_STR_LEN);
      strlcpy(wifiConfig.pwd, json["pwd"], MAX_STR_LEN);
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
    Serial.println("Connecting Wifi...");
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

bool isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool tryConnectWifi(char* ssid, char* pwd) {
  
  bool res = false;
  
  WiFi.begin(ssid, pwd);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  uint8_t attempts = 0;
  while ((!isWifiConnected()) && attempts < WIFI_TIMEOUT) { // Wait for the Wi-Fi to connect
     delay(500);
     Serial.print(".");
     attempts++;
  }
  
  if (attempts < WIFI_TIMEOUT && isWifiConnected() && WiFi.localIP() != IPAddress(0, 0, 0, 0)) {
    Serial.println();
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    WiFi.softAPdisconnect();
    isWifiConfigSet = true;
    
    res = true;
  } else {
    Serial.println();
    Serial.println("Failed to connect to wifi");
    WiFi.disconnect();
  }

  return res;
}

#endif

#ifndef __INIT_WIFI__
#define __INIT_WIFI__

struct WifiConfig {
  char ssid[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
};

const char *wifiFileName = "/wifiConfig.json";
WifiConfig wifiConfig;
bool isWifiConfigSet = false;
bool isAPRunning = false;

void setWifiConfig(const char* ssid, const char* pwd) {
  
  memset(wifiConfig.ssid, 0, MAX_STR_LEN);
  memcpy(wifiConfig.ssid, ssid, MAX_STR_LEN);
  
  memset(wifiConfig.pwd, 0, MAX_STR_LEN);
  memcpy(wifiConfig.pwd, pwd, MAX_STR_LEN);
  
  isWifiConfigSet = true;      
}

bool containsWifiConfig(JsonObject&json) {
  return 
    json.containsKey(WIFI_CONFIG_SSID)
    && json.containsKey(WIFI_CONFIG_PWD);
}

bool loadWifiConfig(){
  bool res = false;

  if(readFile(wifiFileName)) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(fileBuff);
    if (
      json.success()
      && containsWifiConfig(json)
      ) {
      setWifiConfig(json[WIFI_CONFIG_SSID], json[WIFI_CONFIG_PWD]);
      res = true;
    } else {
      Serial.println("Wifi Configuration is corrupted");
    }
  }

  return res;    
}

bool saveWifiConfigToSPIFFS(const char* ssid, const char* pwd) {
  
  bool res = false;
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  
  json[WIFI_CONFIG_SSID] = ssid;
  json[WIFI_CONFIG_PWD] = pwd;
  
  res = saveJson(wifiFileName, json);
  
  if(res) {
    Serial.println("Wifi Config are saved");
  }
  return res;
}

void startAP() {
  Serial.println("Starting Access Point");
  WiFi.softAP(qrConfig.AP_SSID, qrConfig.AP_PWD);
  wifiServer.begin();
  isAPRunning = true;
  Serial.println(WiFi.softAPIP());
}

void stopAP() {
  WiFi.softAPdisconnect();
  wifiServer.begin();
  isAPRunning = false;
  Serial.println("Access Point is stopped");
}


void initWifi() {
  
  loadWifiConfig();
  if(isWifiConfigSet) {
    wifiServer.begin();
    WiFi.begin(wifiConfig.ssid, wifiConfig.pwd);
  } else {
    startAP();
  }
  
}

bool isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool tryConnectWifi(const char* ssid, const char* pwd) {
  
  bool res = false;

  WiFi.disconnect();
  delay(500);
  WiFi.begin(ssid, pwd);
  Serial.print("Connecting to ");
  Serial.print(ssid);

  uint8_t attempts = 0;
  while (!isWifiConnected() && attempts < WIFI_TIMEOUT) { // Wait for the Wi-Fi to connect
     delay(500);
     Serial.print(".");
     attempts++;
  }
  Serial.println();

  //!!! ip adress condition part does not work
  if (attempts < WIFI_TIMEOUT && isWifiConnected() && WiFi.localIP() != IPAddress(0, 0, 0, 0)) {
    isWifiConfigSet = true;
    res = true;
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect to wifi");
  }

  return res;
}

bool reconnectWifi() {
  return tryConnectWifi(wifiConfig.ssid, wifiConfig.pwd);
}

uint8_t saveWifiConfig(JsonObject& json) {
  uint8_t res = 0;
  
  if(
      json.success()
      && containsWifiConfig(json)
  ) {        
    const char* ssid = json[WIFI_CONFIG_SSID];
    const char* pwd = json[WIFI_CONFIG_PWD];
    if(tryConnectWifi(ssid, pwd)) {
      if(saveWifiConfigToSPIFFS(ssid, pwd)) {
        setWifiConfig(ssid, pwd);
        Serial.println("Wifi is configured");
      } else {
        res = SAVE_WIFI_FAILED_RESPONSE_HEADER;
        Serial.println("Saving wifiConfig failed");                
      }
    } else {
      WiFi.disconnect();
      res = WIFI_CONNECTION_FAILED_RESPONSE_HEADER;
      Serial.println("Error. Could not connect to wifi with wifiConfig provided");
    }
  } else {
    res = INVALID_WIFI_CONFIG_RESPONSE_HEADER;
    Serial.println("Error. Invalid wifiConfig is received");
  }
  
  return res;
}

#endif

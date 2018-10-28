#ifndef __INIT_WIFI__
#define __INIT_WIFI__

#include <string.h>

WiFiServer wifiServer(80);

struct WifiConfig {
  char ssid[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
};

const char *wifiName = "/wifiConfig.json";
static WifiConfig wifiConfig;
bool isWifiConfigSet = false;

void setWifiConfig(const char* ssid, const char* pwd) {
  Serial.println("Reset ssid");
  Serial.print("size of wifi.conf ssid: ");
  Serial.println(sizeof(wifiConfig.ssid));
  Serial.print("size of  ssid: ");
  Serial.println(strlen(ssid));
  
  memset(wifiConfig.ssid, 0, MAX_STR_LEN);
  Serial.println("Set new ssid");
  memcpy(wifiConfig.ssid, ssid, MAX_STR_LEN);
  
  Serial.println("Reset pwd");
  Serial.print("size of wifi.conf pwd: ");
  Serial.println(strlen(wifiConfig.pwd));
  Serial.print("size of  pwd: ");
  Serial.println(strlen(pwd));
  memset(wifiConfig.pwd, 0, MAX_STR_LEN);
  Serial.println("Set new pwd");
  memcpy(wifiConfig.pwd, pwd, MAX_STR_LEN);
  isWifiConfigSet = true;      
}

void loadWifiConfig(){

  if (!SPIFFS.begin()){
    Serial.println("Failed to mount FS...");
    return;
  }
  if(!SPIFFS.exists(wifiName)){
    Serial.print(wifiName);
    Serial.println(" does not exist");
    return;
  }

  File wifiFile = SPIFFS.open(wifiName);
  if (wifiFile) 
  {
    Serial.println("Opened file");
    size_t size = wifiFile.size();
    Serial.println("Reading Wifi Config from Flash");
    
    std::unique_ptr<char[]> buf(new char[size]);
    wifiFile.readBytes(buf.get(), size);
    serialStr(buf.get());
    
    //char* b = new char[size];
    //memset(wifiConfig.ssid, '\0', size);
    //wifiFile.readBytes(b, size);
    //serialStr(b);
    //delete[] b;

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());
//    
    if (json.success()) {
      Serial.println("Wifi Configuration");
      json.prettyPrintTo(Serial);
      setWifiConfig(json["wifi_ssid"], json["wifi_pwd"]);
    } else {
      Serial.println("Wifi Configuration is corrupted");
    }
    
    wifiFile.close();
  } else {
      Serial.println("Failed opening Wifi Configuration File");
  }
  
}

void fileStr(File f, const char* str) {
  for(int i = 0; i < strlen(str); i++) {
    f.print(char(str[i]));
  }
  f.println();
}

bool saveWifiConfig(const char* ssid, const char* pwd) {
  
  bool res = false;
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
//  
  json["wifi_ssid"] = ssid;
  json["wifi_pwd"] = pwd;      


//  if(SPIFFS.exists(wifiName)) {
//    SPIFFS.remove(wifiName);
//    delay(250);
//  }
  
  File configFile = SPIFFS.open(wifiName, FILE_WRITE);
  if (configFile) {
    json.printTo(Serial);
    json.printTo(configFile);
    
//    fileStr(configFile, ssid);
//    fileStr(configFile, pwd);

    configFile.close();
    res = true;
  }
  
  return res;
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
  while (!isWifiConnected() && attempts < WIFI_TIMEOUT) { // Wait for the Wi-Fi to connect
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
    //WiFi.disconnect();
  }

  return res;
}

#endif

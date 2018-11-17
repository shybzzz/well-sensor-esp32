#ifndef __CONNECT_WIFI_ROUTINE__
#define __CONNECT_WIFI_ROUTINE__

bool sendWifiConfig(WiFiClient& client) {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["ip"] = WiFi.localIP().toString();
  json["ssid"] = wifiConfig.ssid;

  return writeJson(client, json);
}

bool listenSetWifiConfig(WiFiClient& client) {
  
  bool res = readSocket(client, SET_WIFI_CONFIG_REQUEST_HEADER);

  if(res) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(socketBuff);
    res = 
      json.success() && 
      json.containsKey(WIFI_CONFIG_SSID) &&
      json.containsKey(WIFI_CONFIG_PWD);
    
    if(res) {
      const char* ssid = json[WIFI_CONFIG_SSID];
      const char* pwd = json[WIFI_CONFIG_PWD];
      res = tryConnectWifi(ssid, pwd);
      
      if(res) {
        res = saveWifiConfig(ssid, pwd);
        
        if(res) {
          setWifiConfig(ssid, pwd);
          sendWifiConfig(client);
          
          delay(4000);
          WiFi.softAPdisconnect();
          Serial.println();
          Serial.println("AP is stopped");
        } else {
          Serial.println("Saving wificonfig failed");                
        }
      } else {
        WiFi.disconnect();
        client.print(WIFI_CONNECTION_FAILED_RESPONSE_HEADER);
        Serial.println("Error. Could not connect to wifi with wifiConfig provided");
      }
      
    } else {
      client.print(INVALID_WIFI_CONFIG_RESPONSE_HEADER);
      Serial.println("Error. Invalid wifiConfig is received");
    }
  }

  return res;
}

bool listenGetWifiConfig(WiFiClient& client) {  
  return 
    readSocket(client, GET_IP_REQUEST_HEADER) &&
    sendWifiConfig(client);
}

#endif

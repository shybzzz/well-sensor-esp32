#ifndef __ROUTINE_LISTEN_AP__
#define __ROUTINE_LISTEN_AP__

bool sendWifiInfo(WiFiClient& client) {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["ip"] = WiFi.localIP().toString();
  json["ssid"] = wifiConfig.ssid;

  return writeJson(client, json);
}

bool listenSetConfig(WiFiClient& client) {
  
  bool res = false;

  if(readSocket(client, SET_CONFIG_REQUEST_HEADER)) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(socketBuff);

    int saveWifiConfigRes = saveWifiConfig(json);
    if(saveWifiConfigRes == 0) {
      int saveMqttConfigRes = saveMqttConfig(json);
      if(saveMqttConfigRes == 0) {
        sendWifiInfo(client);          
        
        res = true;
        Serial.println();
        Serial.println("AP is stopped");
      } else {
        client.print(saveMqttConfigRes);
      }
    } else {
      client.print(saveWifiConfigRes);
    }
  }
  
  return res;
}

bool listenGetWifiInfo(WiFiClient& client) {  
  return 
    readSocket(client, GET_IP_REQUEST_HEADER)
    && sendWifiInfo(client);
}

void listenServer() {
  WiFiClient client = wifiServer.available();
  if(isClientConnected(client)) {
    if(isAPRunning) {
      listenSetConfig(client);
      listenGetWifiInfo(client);
    }
  }
  delay(500);
  client.stop();
}

#endif

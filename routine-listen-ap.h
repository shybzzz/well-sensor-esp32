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

    uint8_t wifiConfigEr = handleWifiJson(json);
    if(wifiConfigEr == 0) {
      uint8_t mqttConfigEr = handleMqttJson(json);
      if(mqttConfigEr == 0) {
        sendWifiInfo(client);          
        delay(500);
        stopAP();
        res = true;
        Serial.println();
        Serial.println("AP is stopped");
      } else {
        client.print(mqttConfigEr);
      }
    } else {
      client.print(wifiConfigEr);
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

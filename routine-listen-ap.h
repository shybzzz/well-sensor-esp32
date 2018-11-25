#ifndef __ROUTINE_LISTEN_AP__
#define __ROUTINE_LISTEN_AP__

bool sendConfig(WiFiClient& client) {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[WIFI_CONFIG_IP] = WiFi.localIP().toString();
  json[WIFI_CONFIG_SSID] = wifiConfig.ssid;
  json[MQTT_CONFIG_SERVER] = mqttConfig.server;
  json[MQTT_CONFIG_PORT] = mqttConfig.port;
  json[MQTT_CONFIG_USER] = mqttConfig.user;

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
        sendConfig(client);          
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
    && sendConfig(client);
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

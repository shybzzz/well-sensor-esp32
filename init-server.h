#ifndef __SOCKETS__
#define __SOCKETS__

#define MAX_SOCKET_BUFF_SIZE 1024u

char socketBuff[MAX_SOCKET_BUFF_SIZE] = {0};
uint8_t requestHeader = 0;

void printSocketBuff() {
  for (int i = 0; i < MAX_SOCKET_BUFF_SIZE; i++) {
    Serial.print(char(socketBuff[i]));
  }
  Serial.println();
}

bool readSocket(WiFiClient& client, uint8_t dataType) {
  bool res = false;
  uint16_t msgLength = client.available();

  if (msgLength > 0 && dataType == requestHeader) {
    memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
    char* tmp = nullptr;

    tmp = new char[msgLength];
    client.read(reinterpret_cast<uint8_t*>(tmp), msgLength);
    memcpy(socketBuff , tmp, msgLength);
    delete[] tmp;
    tmp = nullptr;
    res = true;
  }

  return res;
}

bool writeJson(WiFiClient& client, JsonObject& json) {

  memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
  json.printTo(socketBuff);
  client.write(socketBuff, json.measureLength());
  Serial.println("Data sent to client");
  json.prettyPrintTo(Serial);
  Serial.println();

  return true;

}

bool isClientConnected(WiFiClient& client) {
  bool res = false;
  if (client.connected()) {
    Serial.print("Client Connected: ");
    delay(500);
    if (client.available()) {
      requestHeader = client.read();
      Serial.println(requestHeader);
      res = true;
    }
  }
  return res;
}

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

  if (readSocket(client, SET_CONFIG_REQUEST_HEADER)) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(socketBuff);
    handleSensorJson(json);

    uint8_t wifiConfigEr = handleWifiJson(json);
    if (wifiConfigEr == 0) {
      uint8_t mqttConfigEr = handleMqttJson(json);
      if (mqttConfigEr == 0) {
        sendConfig(client);        
        delay(500);
        stopAP();
        res = true;
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
  if (isClientConnected(client)) {
    if (isAPRunning) {
      listenSetConfig(client);
      listenGetWifiInfo(client);
    }
  }
  delay(500);
  client.stop();
}

#endif

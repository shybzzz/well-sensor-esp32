#ifndef __CONNECT_WIFI_ROUTINE__
#define __CONNECT_WIFI_ROUTINE__

void sendIpJson(WiFiClient& client) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["ip"] = WiFi.localIP().toString();
  json["ssid"] = wifiConfig.ssid;

  json.printTo(client);

  delay(1000);
  client.stop();
  Serial.println("Data sent to client");
}

bool listenSetWifiConfig(WiFiClient& client) {
  
  bool res = readSocket(client, SET_WIFI_CONFIG_REQUEST_HEADER);

  if(res) {
    char output[2][MAX_STR_LEN];
    memset(output[0], '\0', MAX_STR_LEN);
    memset(output[1], '\0', MAX_STR_LEN);
    res = parse(socketBuff, output);
    
    if(res) {
      res = tryConnectWifi(output[0], output[1]);
      if(res) {
        res = saveWifiConfig(output[0], output[1]);
        if(res) {
          setWifiConfig(output[0], output[1]);
          sendIpJson(client);
          delay(4000);
          WiFi.softAPdisconnect();
          Serial.println("AP is stopped");
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

bool listenGetIp(WiFiClient& client) {
  bool res = readSocket(client, GET_IP_REQUEST_HEADER);

  if(res) {
    sendIpJson(client);
  }

  return res;
}

#endif

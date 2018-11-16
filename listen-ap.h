#ifndef __CONNECT_WIFI_ROUTINE__
#define __CONNECT_WIFI_ROUTINE__

bool sendIp(WiFiClient& client){
  Serial.println("Sending Data...");

          memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
          socketBuff[0] = SUCCESS_RESPONSE_RESULT;
          socketBuff[1] = WIFI_CONFIG_SUCCESS_RESPONSE_HEADER;
          const char* ip = WiFi.localIP().toString().c_str();
          strcpy(socketBuff + 2, ip);

          client.print(socketBuff);
          Serial.println(socketBuff);

          delay(750);
          client.stop();
          Serial.println();
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
          Serial.println();

          Serial.println("Sending Data...");

          memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
          socketBuff[0] = SUCCESS_RESPONSE_RESULT;
          socketBuff[1] = WIFI_CONFIG_SUCCESS_RESPONSE_HEADER;
          const char* ip = WiFi.localIP().toString().c_str();
          strcpy(socketBuff + 2, ip);

          client.print(socketBuff);
          Serial.println(socketBuff);

          delay(750);
          client.stop();
          sendIp(client);
          delay(4000);
          WiFi.softAPdisconnect();
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
    sendIp(client);
  }

  return res;
}

#endif

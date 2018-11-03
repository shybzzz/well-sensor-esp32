#ifndef __CONNECT_WIFI_ROUTINE__
#define __CONNECT_WIFI_ROUTINE__

bool readWifiConfig(WiFiClient& client) {
  
  bool res = readSocket(client, WIFI_CONFIG_REQUEST_HEADER);

  if(res) {
    char output[2][MAX_STR_LEN];
    memset(output[0], '\0', MAX_STR_LEN);
    memset(output[1], '\0', MAX_STR_LEN);
    res = parse(socketBuff, output);
    
    if(res) {
      res = tryConnectWifi(output[0], output[1]);
      if(res) {
//        res = saveWifiConfig(output[0], output[1]);
        if(res) {
          setWifiConfig(output[0], output[1]);
          Serial.println();
          Serial.println("Sending Data...");

          memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
          memset(socketBuff, SUCCESS_RESPONSE_RESULT, 1);
          memset(socketBuff + 1, WIFI_CONFIG_SUCCESS_RESPONSE_HEADER, 1);
          const char* ip = WiFi.localIP().toString().c_str();
          strcpy(socketBuff + 2, ip);

          client.print(socketBuff);
          serialStr(socketBuff);

          delay(750);
          client.stop();
          delay(4000);
          WiFi.softAPdisconnect();
          Serial.println();
          Serial.println("Data sent to client");
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

#endif

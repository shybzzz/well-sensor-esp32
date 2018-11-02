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
          String ip = WiFi.localIP().toString();
          Serial.println(ip);
          
          client.write((uint8_t)SUCCESS_RESPONSE_RESULT);
          client.write((uint8_t)WIFI_CONFIG_SUCCESS_RESPONSE_HEADER);
          client.write(ip.c_str(), strlen(ip.c_str()));
          
          delay(1000);
          client.stop();
          delay(250);
          WiFi.softAPdisconnect();
          Serial.println("Data sent to client");
        }                
      } else {
        client.write(WIFI_CONNECTION_FAILED_RESPONSE_HEADER);
        Serial.println("Error. Could not connect to wifi with wifiConfig provided");
      }
    } else {
      client.write(INVALID_WIFI_CONFIG_RESPONSE_HEADER);
      Serial.println("Error. Invalid wifiConfig is received");
    }
  }

  return res;
}

#endif

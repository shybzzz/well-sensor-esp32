#ifndef __ROUTINE_LISTEN_AP__
#define __ROUTINE_LISTEN_AP__

bool sendWifiConfig(WiFiClient& client) {
  
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json["ip"] = WiFi.localIP().toString();
  json["ssid"] = wifiConfig.ssid;

  return writeJson(client, json);
}

bool listenSetWifiConfig(WiFiClient& client) {
  
  bool res = false;

  if(readSocket(client, SET_WIFI_CONFIG_REQUEST_HEADER)) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(socketBuff);
    
    if(
      json.success()
      && containsWifiConfig(json)
      && containsMqttConfig(json)
    ) {
        
      const char* ssid = json[WIFI_CONFIG_SSID];
      const char* pwd = json[WIFI_CONFIG_PWD];
      const char* server = json[MQTT_CONFIG_SERVER];
      int port = json[MQTT_CONFIG_PORT];
      const char* user = json[MQTT_CONFIG_USER];
      const char* mqttPwd = json[MQTT_CONFIG_PWD];
      
      if(
        tryConnectWifi(ssid, pwd)
        && tryConnectMqtt(server, port, user, mqttPwd)
      ) {
        if(
          saveWifiConfig(ssid, pwd)
          && saveMqttConfig(server, port, user, mqttPwd)
        ) {
          setWifiConfig(ssid, pwd);
          setMqttConfig(server, port, user, mqttPwd);          
          sendWifiConfig(client);          
          WiFi.softAPdisconnect();
          res = true;
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
    readSocket(client, GET_IP_REQUEST_HEADER)
    && sendWifiConfig(client);
}

#endif

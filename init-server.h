#ifndef __SOCKETS__
#define __SOCKETS__

#define MAX_SOCKET_BUFF_SIZE 1024u

WiFiServer wifiServer(80);
char socketBuff[MAX_SOCKET_BUFF_SIZE] = {0};
bool isAPMode = false;
char requestHeader;

void printSocketBuff() {
  for(int i=0; i < MAX_SOCKET_BUFF_SIZE; i++) {
    Serial.print(char(socketBuff[i]));
  }
  Serial.println();
}

bool readSocket(WiFiClient& client, char dataType) {
  bool res = false;
  uint16_t msgLength = client.available();
  
  if(msgLength > 0 && dataType == requestHeader) {
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

#endif

#ifndef __INIT_SOCKETS__
#define __INIT_SOCKETS__

#define MAX_SOCKET_BUFF_SIZE 100u

char socketBuff[MAX_SOCKET_BUFF_SIZE] = {0};

bool readSocket(WiFiClient& client, uint8_t dataType) {
  bool res = false;
  memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
  char* tmp = nullptr;
  if(client && client.connected()) {
    Serial.println("Client Connected");
    delay(250);
    Serial.print("Message Length: ");
    Serial.println(client.available());
    if(client.available()) {
      byte msgType = client.read();
      if(msgType == dataType) {
        Serial.print("Data Type: ");
        Serial.println(dataType);
        uint16_t msgLength = client.available();
        Serial.print("Message Length: ");
        Serial.println(msgLength);
        tmp = new char[msgLength];
        client.read(reinterpret_cast<uint8_t*>(tmp), msgLength);
        memcpy(socketBuff , tmp, msgLength);
        res = true;
      } else {
        Serial.print("Not supported message type: ");
        Serial.println(msgType);
        client.write(dataType);
      }
    } else {
      client.write(dataType);
    }
    delay(250);
    client.stop();      
  }

  delete[] tmp;
  return res;
}

void printSocketBuff() {
  for(int i=0; i < MAX_SOCKET_BUFF_SIZE; i++) {
    Serial.print(char(socketBuff[i]));
  }
}

#endif

#ifndef __SOCKETS__
#define __SOCKETS__

#define MAX_SOCKET_BUFF_SIZE 1024u

char socketBuff[MAX_SOCKET_BUFF_SIZE] = {0};

void printSocketBuff() {
  for(int i=0; i < MAX_SOCKET_BUFF_SIZE; i++) {
    Serial.print(char(socketBuff[i]));
  }
  Serial.println();
}

bool readSocket(WiFiClient& client, char dataType) {
  
  bool res = false;
  memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
  char* tmp = nullptr;
  
  if (client.connected()) {
    
    Serial.println("Client Connected");
    delay(750);
    
    if (client.available()) {
      
    char msgType = client.read();
    Serial.print("received: ");
    Serial.println(msgType);
    if (msgType == dataType) {
        
       Serial.print("Data Type: ");
       Serial.println(dataType);
        
       uint16_t msgLength = client.available();
        
       tmp = new char[msgLength];
       client.read(reinterpret_cast<uint8_t*>(tmp), msgLength);
       memcpy(socketBuff , tmp, msgLength);
      
       res = true;
       delete[] tmp;
       tmp = nullptr;
        
     } else {
        
       Serial.print("Not supported message type: ");
       Serial.println(msgType);                
     }
      
    }          
  }
 
  return res; 
}

bool writeJson(WiFiClient& client, JsonObject& json) {
  
  memset(socketBuff, '\0', MAX_SOCKET_BUFF_SIZE);
  json.printTo(socketBuff);
  client.write(socketBuff, json.measureLength());

  delay(1000);
  client.stop();
  Serial.println("Data sent to client");
  json.prettyPrintTo(Serial);
  
  return true;
  
}



#endif

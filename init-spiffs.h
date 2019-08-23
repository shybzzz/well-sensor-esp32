#ifndef __INIT_SPIFFS__
#define __INIT_SPIFFS__

#define MAX_FILE_BUFF_SIZE 1024u

char fileBuff[MAX_FILE_BUFF_SIZE] = {0};

bool readFile(const char* fileName) {
  bool res = false;
  memset(fileBuff, '\0', MAX_FILE_BUFF_SIZE);
  
  if (SPIFFS.begin()){
    if(SPIFFS.exists(fileName)) {
      File f = SPIFFS.open(fileName);
      if(f) {
        size_t s = f.size();
        f.readBytes(fileBuff, s);
        f.close();
        res = true;
      } else {
        Serial.println("Failed opening file");
      }
    } else {
      Serial.print(fileName);
      Serial.println(" does not exist");
    }
  } else {
    Serial.println("Failed to mount FS...");
  }

  return res;
}

bool saveJson(const char* fileName, JsonObject& json) {
  bool res = false;

  if(SPIFFS.exists(fileName)){
    if(!SPIFFS.remove(fileName)) {
      Serial.print("Failed Removing ");
      Serial.println(fileName);
      return res;
    }
  }
  
  File f = SPIFFS.open(fileName, FILE_WRITE);
  if (f) {
    serializeJson(json, f);
    //json.printTo(f);    
    f.close();
    res = true;
  } else {
    Serial.print("Failed opening ");
    Serial.println(fileName);
  }
  
  return res;
}

bool initSPIFSS() {
  bool res = false;

  if(SPIFFS.begin()) {
    res = true;
    Serial.println("File System is mounted");
  } else {
    Serial.println("Failed to Mount File System. Formatting...");
    SPIFFS.format();
    delay(1000);
    ESP.restart();
  }

  return res;
}


#endif

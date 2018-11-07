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
        serialStr(fileBuff);
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
  
  File f = SPIFFS.open(fileName, FILE_WRITE);
  if (f) {
    json.printTo(f);    
    f.close();
    res = true;
  } else {
    Serial.print("Failed opening ");
    Serial.println(fileName);
  }
  
  return res;
}

#endif

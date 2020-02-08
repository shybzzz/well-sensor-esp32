#ifndef __INIT_OTA__
#define __INIT_OTA__

const char* espUpdatesFile = "/updates.json";

// OTA Logic 
bool execOTA(JsonObject& json) 
{
  Serial.println("ExecOTA...");
  HTTPClient http;
  // Connect to S3
  
  http.begin(json[UPDATE_URL]);

  int status = http.GET();
  Serial.print("Code: ");
  Serial.println(status);  
  if (status == HTTP_CODE_OK)
  {
    // get lenght of document (is -1 when Server sends no Content-Length header)
    int len = http.getSize();
    Serial.print("Read bytes: ");
    Serial.println(len);
     
    bool canBegin = Update.begin(len);
    Serial.print("Error code from Update lib: ");
    Serial.println(Update.getError());
    // If yes, begin
    if (canBegin) 
    {
      Serial.println("Begin OTA. This may take 2 - 5 mins to complete. Things might be quite for a while.. Patience!");
      // No activity would appear on the Serial monitor
      // So be patient. This may take 2 - 5mins to complete
      auto stream = http.getStreamPtr();
      //http.writeToStream(stream);
      Serial.println("Created WiFiClient&...");
      Serial.print("Bytes available in socket: ");
      Serial.println(stream->available());
      size_t written = Update.writeStream(*stream);

      if (written == len) 
      {
        Serial.println("Written : " + String(written) + " successfully");
      } else 
      {
        Serial.println("Written only : " + String(written) + "/" + String(len) + ". Retry?" );
        return false;
      }

      if (Update.end()) 
      {
        Serial.println("OTA done!");
        if (Update.isFinished()) 
        {
          Serial.println("Update successfully completed. Rebooting.");
          return true;
        } else 
        {
          Serial.println("Update not finished? Something went wrong!");
        }
      } else {
        Serial.println("Error Occurred. Error #: " + String(Update.getError()));
      }
    } else {
      // not enough space to begin OTA
      // Understand the partitions and
      // space availability
      Serial.println("Not enough space to begin OTA");
      http.end();
    }
  }
  return false;
}

void saveUpdateConfigsJson(JsonObject& json)
{
  bool res = saveJson(espUpdatesFile, json);

  if (res) {
    Serial.println("ESP32 update configs are saved.");  
  }
}

void handleEspUpdateJson(JsonObject& json) {
  if (!json.isNull() && execOTA(json)) {
      saveUpdateConfigsJson(json);
      ESP.restart(); 
  }  
}
#endif

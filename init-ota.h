#ifndef __INIT_OTA__
#define __INIT_OTA__

const char* espUpdatesFile = "/updateInfo.json";

// OTA Logic
bool execOTA(JsonObject& updateInfo)
{
  Serial.println("ExecOTA...");
  HTTPClient http;
  http.begin(updateInfo[UPDATE_URL]);

  int status = http.GET();
  Serial.print("Code: ");
  Serial.println(status);
  if (status == HTTP_CODE_OK)
  {
    // get lenght of document (is -1 when Server sends no Content-Length header)
    int len = http.getSize();
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
      size_t written = Update.writeStream(*stream);

      if (written == len)
      {
        Serial.println("Written : " + String(written) + " successfully");
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

void saveUpdateConfigsJson(JsonObject& updateInfo)
{
  bool res = saveJson(espUpdatesFile, updateInfo);

  if (res) {
    Serial.println("ESP32 update configs are saved.");
  }
}

void handleEspUpdateJson(JsonObject& updateInfo) {
  if (!updateInfo.isNull() && execOTA(updateInfo)) {
    saveUpdateConfigsJson(updateInfo);
    ESP.restart();
  }
}
#endif

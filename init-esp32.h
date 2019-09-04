#ifndef __INIT_ESP32__
#define __INIT_ESP32__

#define DEFAULT_DELAY_TIME  100 //in ms.

struct EspConfigs
{
  uint16_t delayTime;  
};

EspConfigs espConfig;

const char* espConfigFile = "/espConfig.json";

bool setEsp32Config(JsonObject& json)
{
  if (json.containsKey(MEASUREMENT_DELAY)) {
    
    espConfig.delayTime = json[MEASUREMENT_DELAY];
    return true;  
    }
    
  return false;
}

bool saveEspConfigsToSPIFFS() {

    bool res = false;

    DynamicJsonDocument jsonBuffer{MAX_STR_LEN * 2};
    JsonObject json = jsonBuffer.to<JsonObject>();

    json[MEASUREMENT_DELAY] = espConfig.delayTime;

    res = saveJson(espConfigFile,json);

    if (res) {
      Serial.println("ESP32 configs are saved.");  
    }

    return res;
}

void handleEspJson(JsonObject& json) {
  if (!json.isNull() && setEsp32Config(json)) {
    saveEspConfigsToSPIFFS(); 
    
    Serial.print("Meaurements delayTime = ");
    Serial.println(espConfig.delayTime); 
  }  
}

bool loadEspConfig()
{
  bool res = false;

  if (readFile(espConfigFile)) {
      res = true;  
  }
  else {
      Serial.println("Invalid json file.");  
  }

  return res;
}

void initEsp32()
{
  if (loadEspConfig()) {

      Serial.println("Found valid ESP32 configs");
            
      DynamicJsonDocument jsonBuffer{MAX_STR_LEN * 2};
      deserializeJson(jsonBuffer, fileBuff);
      JsonObject json = jsonBuffer.as<JsonObject>();
      setEsp32Config(json);
  }
  else {
      espConfig.delayTime = DEFAULT_DELAY_TIME;
      Serial.print("Default Meaurements delayTime = ");
      Serial.println(espConfig.delayTime);
  }  
}
#endif

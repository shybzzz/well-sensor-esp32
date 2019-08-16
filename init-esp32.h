#ifndef __INIT_ESP32__
#define __INIT_ESP32__

#define DEFAULT_DELAY_TIME  100 //in ms.

struct EspConfigs
{
  uint16_t delayTime;  
};

EspConfigs espConfig;

const char* espConfigFile = "/espConfig.json";


bool loadEspConfig()
{
  bool res = false;

  if (readFile(espConfigFile)) {
      
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(fileBuff);

      if (json.success()) {
          
          if (json.containsKey(MEASUREMENT_DELAY)) {
            espConfig.delayTime = json[MEASUREMENT_DELAY];  
          }
          
         res = true;  
      }
      else{
        Serial.println("Invalid json file.");  
      }
  }

  return res;
}

void initEsp32()
{
  if (loadEspConfig()) {

      Serial.println("Found valid ESP32 configs");
      Serial.print("Meaurements delayTime = ");
      Serial.println(espConfig.delayTime);
  }
  else {
      espConfig.delayTime = DEFAULT_DELAY_TIME;
      Serial.print("Default Meaurements delayTime = ");
      Serial.println(espConfig.delayTime);
  }  
}

bool saveEspConfigsToSPIFFS() {

    bool res = false;

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();

    json[MEASUREMENT_DELAY] = espConfig.delayTime;

    res = saveJson(espConfigFile,json);

    if (res) {
      Serial.println("ESP32 configs are saved.");  
    }

    return res;
}

void handleEspJson(JsonObject& json) {
  if (json.success()) {
    saveEspConfigsToSPIFFS();  
  }  
}
#endif

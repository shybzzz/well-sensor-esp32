#ifndef __INIT_COMBINED_SENSOR__
#define __INIT_COMBINED_SENSOR__

const char *sensorFileName = "/sensorConfig.json";

bool containsSensorConfig(JsonObject& json)
{
  return 
        json.containsKey(SENSOR_CONFIG_TYPE);  
}

bool loadSensorConfig()
{
  bool res = false;
  
  if (readFile(sensorFileName)) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(fileBuff);
  if (
    json.success()
    && containsSensorConfig(json)
  ) {
    res = true;
  } else {
    Serial.println("Unsuported sensor config type!");
  }
  } else {
    Serial.println("Unsuported sensor config type!");
  }
  
  return res;
}
void initSensors()
{
  if (loadSensorConfig())
  {
    Serial.println("Found valid sensor configs. Init combined sensors.");
    powerMeters[0].init(powerMeterConfigs[0]);
    initDallasSensor();
    initADC();
  }
}
int gatherData(int val, int* data) {
  if (current_sample < DATA_SIZE) {
    data[current_sample] = val;    
  } else {
    for (int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = val;
  }
  return val;
}

bool gatherAllData() {
  gatherData(powerMeters[1].readCurrent(), dataCharging);
  gatherData(powerMeters[0].readPower(), dataConsumption);
  gatherData(getADC_Data(), dataDepth);
//  TODO: Ask Sych about that
  gatherData(getMedianData(), dataDischarge);
  int t = getDallasTempData();
  gatherData(t, dataTemperature);
  
  bool res = current_sample >= DATA_SIZE;
  
  return res;
}

bool measure()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  gatherAllData();
  
  json[PAYLOAD_DISCHARGE] = dataDischarge[DATA_SIZE - 1];
  bool res = current_sample >= DATA_SIZE;
  if (res) {
    json[PAYLOAD_INA260_CONSUMPTION] = filterMedian(dataConsumption);
    json[PAYLOAD_INA260_CHARGING] = filterMedian(dataCharging);
    int t = filterMedian(dataTemperature);
    json[PAYLOAD_DS18B20] = t;
    json[PAYLOAD_GUT800] = filterMedian(dataDepth);    
  } else {
    json[PAYLOAD_INA260_CONSUMPTION] = dataConsumption[current_sample];
    json[PAYLOAD_INA260_CHARGING] = dataCharging[current_sample];
    int t = dataTemperature[current_sample];
    json[PAYLOAD_DS18B20] = t;
    json[PAYLOAD_GUT800] = dataDepth[current_sample];
    
  }

  JsonObject& root = jsonBuffer.createObject();
  root[PAYLOAD_VALUE] = json;
  publishJson(TOPIC_DATA, root);

  if(!res) {
    current_sample++;
  }
  
  //root.prettyPrintTo(Serial);
  //Serial.println();
  return res;
}

bool saveSensorConfigToSPIFFS() {

  bool res = false;

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[SENSOR_CONFIG_TYPE] = SENSOR_COMBINED;

  json[POWER_ADDR] = powerMeterConfigs[0].addr;
  json[POWER_MODE] = powerMeterConfigs[0].configReg.mode;
  json[POWER_ISHCT] = powerMeterConfigs[0].configReg.ishct;
  json[POWER_VBUSCT] = powerMeterConfigs[0].configReg.vbusct;
  json[POWER_AVG] = powerMeterConfigs[0].configReg.avg;
  json[POWER_RST] = powerMeterConfigs[0].configReg.rst;

  res = saveJson(sensorFileName, json);

  if (res) {
    Serial.println("Sensor Config are saved");
  }
  return res;
}

void handleSensorJson(JsonObject& json) {
  if (
    json.success()
    && containsSensorConfig(json)
  ) {
    uint8_t sensorType = json[SENSOR_CONFIG_TYPE];
    saveSensorConfigToSPIFFS();
  }
}

#endif

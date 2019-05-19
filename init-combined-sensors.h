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
  }
}
int gatherData(int val, int* data) {
  if (current_sample < DATA_SIZE) {
    data[current_sample] = val;
    //current_sample++;
  } else {
    for (int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = val;
  }
  return val;
}

bool measure()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  int val = powerMeters[0].readVoltage();
  Serial.print("Voltage: ");
  Serial.println(val);
  gatherData(val, dataConsumption);

  val = getDallasTempData();
  Serial.print("\nTemper: ");
  Serial.println(val);
  gatherData(val, dataTemperature);
  
  current_sample++;
  
  bool res = current_sample >= DATA_SIZE;
  if (res) {
    json[PAYLOAD_INA260_CONSUMPTION] = filterMedian(dataConsumption);
   // json[PAYLOAD_INA260_CHARGING] = filterMedian(dataCharging);
    json[PAYLOAD_DS18B20] = filterMedian(dataTemperature);
   // json[PAYLOAD_GUT800] = filterMedian(dataDepth);
   // json[PAYLOAD_DISCHARGE] = filterMedian(dataDischarge);
  }

  JsonObject& root = jsonBuffer.createObject();
  root[PAYLOAD_VALUE] = json;
  publishJson(TOPIC_DATA, root);
  root.printTo(Serial);
  
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

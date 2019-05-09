#ifndef __INIT_SENSOR__
#define __INIT_SENSOR__

using getData = int();

struct SensorConfig {
  uint8_t sensorType;
};

const char *sensorFileName = "/sensorConfig.json";
SensorConfig sensorConfig;

bool containsSensorConfig(JsonObject&json) {
  return
    json.containsKey(SENSOR_CONFIG_TYPE);
}

void setSensorConfig(uint8_t sensorType) {
  sensorConfig.sensorType = sensorType;
}

void loadSensorConfig() {
  if (readFile(sensorFileName)) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(fileBuff);
    if (
      json.success()
      && containsSensorConfig(json)
    ) {
      setSensorConfig(json[SENSOR_CONFIG_TYPE]);
    } else {
      setSensorConfig(SENSOR_SIMULATED);
    }
  } else {
    setSensorConfig(SENSOR_SIMULATED);
  }
}

void initSensorType() {
  switch (sensorConfig.sensorType) {
    case SENSOR_DS18B20:
      initDallasSensor();
      break;
    case SENSOR_ANALOG_TEST:
      initADC();
      break;
    case SENSOR_INA260_VOLTAGE:
    case SENSOR_INA260_CURRENT:
    case SENSOR_INA260_POWER:
//      InitCurrentSensor();
      break;
  }
}

void initSensor() {
  loadSensorConfig();
  initSensorType();
}

int gatherData(getData func) {
  int d = func();
  if (current_sample < DATA_SIZE) {
    data[current_sample] = d;
    current_sample++;
  } else {
    for (int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = d;
  }
  return d;
}

void printData() {
  for (int lc = 0; lc < DATA_SIZE; lc++) {
    Serial.print(data[lc]);
    Serial.print(", ");
  }
  Serial.println();
}

bool runDataRoutine(getData func) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[PAYLOAD_VALUE_DATA] = gatherData(func);

  bool res = current_sample >= DATA_SIZE;
  if (res) {
    json[PAYLOAD_VALUE_MEDIAN] = filterMedian();
    json[PAYLOAD_VALUE_MEAN] = filterMean();
    json[PAYLOAD_VALUE_EXP_SMOOTH] = filterExpSmooth();
  }

  JsonObject& root = jsonBuffer.createObject();
  root[PAYLOAD_VALUE] = json;
  publishJson(TOPIC_DATA, root);
  //  root.prettyPrintTo(Serial);
  //  printData();
  return res;
}

bool measure() {
  uint8_t sensorType = sensorConfig.sensorType;
  
  return runDataRoutine(
           sensorType == SENSOR_SIMULATED ? getMedianData
           : sensorType == SENSOR_ANALOG_TEST
           || sensorType == SENSOR_GUT800 ? getADC_Data
//           : sensorType == SENSOR_INA260_VOLTAGE ? readVoltage
//           : sensorType == SENSOR_INA260_CURRENT ? readCurrent
//           : sensorType == SENSOR_INA260_POWER   ? readPower
           : sensorType == SENSOR_DS18B20 ? getDallasTempData
           : getMedianData
         );
}

bool saveSensorConfigToSPIFFS(uint8_t sensorType) {

  bool res = false;

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[SENSOR_CONFIG_TYPE] = sensorType;
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
    if (saveSensorConfigToSPIFFS(sensorType)) {
      setSensorConfig(sensorType);
      initSensorType();
    }
  }
}

#endif

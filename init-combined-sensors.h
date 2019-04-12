#ifndef __INIT_COMBINED_SENSOR__
#define __INIT_COMBINED_SENSOR__

void initSensors()
{
  powerMeterConsumption.init();
  initDallasSensor();
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

  int val = powerMeterConsumption.readVoltage();
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

void handleSensorJson(JsonObject& json)
{
  
}
#endif

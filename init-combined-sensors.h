#ifndef __INIT_COMBINED_SENSOR__
#define __INIT_COMBINED_SENSOR__

int gatherData(getData func, int* data) {
  int d = func();
  if (current_sample < DATA_SIZE) {
    data[current_sample] = d;
    //current_sample++;
  } else {
    for (int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = d;
  }
  return d;
}

bool measure()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

//current sapmle is updated in every measure. update once per cycle
//create array of callbacks. or any other solution?
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; ++i)
  {
      gatherData(func, allData[i]);
  }
  current_sample++;
  
  bool res = current_sample >= DATA_SIZE;
  if (res) {
    json[PAYLOAD_INA260_CONSUMPTION] = filterMedian(dataConsumption);
    json[PAYLOAD_INA260_CHARGING] = filterMedian(dataCharging);
    json[PAYLOAD_DS18B20] = filterMedian(dataTemperature);
    json[PAYLOAD_GUT800] = filterMedian(dataDepth);
    json[PAYLOAD_DISCHARGE] = filterMedian(dataDischarge);
  }

  JsonObject& root = jsonBuffer.createObject();
  root[PAYLOAD_VALUE] = json;
  publishJson(TOPIC_DATA, root);
  
}
#endif

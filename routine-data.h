#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();

int gatherData(getData func) {  
  int d = func();
  if(current_sample < DATA_SIZE) {
    data[current_sample] = d;
    current_sample++;
  } else {
    for(int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = d;
  }
  return d;
}

void printData() {
  for(int lc = 0; lc < DATA_SIZE; lc++) {
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
  printData();
  return res;
}

#endif

#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

void gatherData() {  
  data[current_sample++] = getMedianData();
}

int processData() {
  int res = filterMedian();
  current_sample = 0;
  return res;
}

bool runDataRoutine() {
  bool res = false;
  
  if(current_sample<DATA_SIZE) {
    gatherData();
  } else {
    char payload[5];
    sprintf(payload, "%d", processData());
    Serial.print("Data sent to MQTT: ");
    Serial.println(payload);
    mqttClient.publish("data", payload);
    res = true;
  }

  return res;
}

#endif

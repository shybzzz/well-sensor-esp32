#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();

bool gatherData(getData func) {  
  int d = func();
  publishInt(TOPIC_DATA, d);
  bool res = current_sample >= DATA_SIZE;
  if(!res) {
    data[current_sample] = d;
    current_sample++;
  } else {
    for(int lc = 1; lc < DATA_SIZE; lc++) {
      data[lc - 1] = data[lc];
    }
    data[DATA_SIZE - 1] = d;
  }
  return res;
}

void printData() {
  for(int lc = 0; lc < DATA_SIZE; lc++) {
    Serial.print(data[lc]);
    Serial.print(", ");
  }
  Serial.println();
}

bool runDataRoutine(getData func) {
  bool res = gatherData(func);  
  if (res) {
    publishInt(TOPIC_FILTER_EXP_SMOOTH, filterExpSmooth());
    publishInt(TOPIC_FILTER_MEDIAN, filterMedian());
    publishInt(TOPIC_FILTER_MEAN, filterMean());
  }
//  printData();
  return res;
}

#endif

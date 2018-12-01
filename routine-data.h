#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();
using Filter = int();

void gatherData(getData func) {  
  int d = func();
  publishInt("data", d);
  data[current_sample] = d;
  Serial.println("Gather data:");
  for (int i = 0; i < DATA_SIZE; ++i)
  {
    Serial.print(data[i]);
    Serial.print("; ");  
  }
  Serial.print("Current_sample = ");
  Serial.println(current_sample++);
}

int processData(Filter filter) {
  int res = filter();
  current_sample = 0;
  return res;
}

bool runDataRoutine(getData func) {
  bool res = false;
  
  if (current_sample < DATA_SIZE) {
    Serial.println("current_sample < DATA_SIZE");
    gatherData(func);
  } else {
    Serial.println("current_sample == DATA_SIZE");

    int filtered = processData(filterExpSmooth);
    publishInt("filtered/expSmooth", filtered);
    filtered = processData(filterMedian);
    publishInt("filtered/Median", filtered);
    
    res = true;
  }
  return res;
}

#endif

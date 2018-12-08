#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();

void gatherData(getData func) {  
  int d = func();
  publishInt("data", d);
  data[current_sample++] = d;
}

int processData() {
  int res = filterMedian();
  current_sample = 0;
  return res;
}

bool runDataRoutine(getData func) {
  bool res = false;
  
  if(current_sample<DATA_SIZE) {
    gatherData(func);
  } else {
    publishInt("filtered", processData());
    res = true;
  }

  return res;
}

#endif

#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

void gatherData() {  
  int d = getMedianData();
  publishInt("data", d);
  data[current_sample++] = d;
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
    publishInt("filtered", processData());
    res = true;
  }

  return res;
}

#endif

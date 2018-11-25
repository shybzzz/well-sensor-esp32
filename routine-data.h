#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();
using Filter = int();

void gatherData(getData func) {  
  int d = func();
  publishInt("data", d);
  data[current_sample++] = d;
}

int processData(Filter filter) {
  int res = filter();
  current_sample = 0;
  return res;
}

bool runDataRoutine(getData func, Filter filter, int window_size) {
  bool res = false;
  
  if (current_sample < DATA_SIZE && current_sample < window_size) {
    gatherData(func);
  } else {
    publishInt("filtered", processData(filter));
    res = true;
  }

  return res;
}

#endif

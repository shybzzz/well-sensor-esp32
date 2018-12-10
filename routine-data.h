#ifndef __ROUTINE_DATA__
#define __ROUTINE_DATA__

using getData = int();

void gatherData(getData func) {  
  int d = func();
  publishInt("data", d);
  data[current_sample] = d;
  current_sample++;
}

bool runDataRoutine(getData func) {
  bool res = false;
  
  if (current_sample < DATA_SIZE) {
    gatherData(func);
  } else {
    publishInt("filter/expSmooth", filterExpSmooth());
    publishInt("filter/Median", filterMedian());
    publishInt("filter/Mean", filterMean());
    current_sample = 0;    
    res = true;
  }
  return res;
}

#endif

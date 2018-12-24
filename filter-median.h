#ifndef __FILTER_MEDIAN__
#define __FILTER_MEDIAN__

#include <algorithm>

#define MEDIAN_MID DATA_SIZE/2u

int filterMedian()
{
  int res = 0;
  int d[DATA_SIZE];
  std::copy(data, data + DATA_SIZE, d);
  std::sort(d, d + DATA_SIZE);

  res = DATA_SIZE % 2 == 0
    ? (d[MEDIAN_MID-1] + d[MEDIAN_MID]) / 2
    : d[MEDIAN_MID];
    
  return res;
}

#endif

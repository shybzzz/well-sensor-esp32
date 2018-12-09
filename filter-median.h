#ifndef __FILTER_MEDIAN__
#define __FILTER_MEDIAN__

#include <algorithm>

#define MEDIAN_MID DATA_SIZE/2u

int filterMedian()
{
  int res = 0;

  std::sort(data, data + DATA_SIZE);

  if (DATA_SIZE % 2 == 0)
  {
      res = data[MEDIAN_MID-1] + data[MEDIAN_MID];
      res /=2;
  }
  else
  {
      res = data[MEDIAN_MID];
  }
  return res;
}

#endif

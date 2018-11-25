#ifndef __FILTER_MEDIAN__
#define __FILTER_MEDIAN__

#include <algorithm>

int filterMedian()
{
  int res = 0;

  std::sort(data, data + MEDIAN_WINDOW);

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

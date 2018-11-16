#ifndef __FILTER_MEDIAN__
#define __FILTER_MEDIAN__

#include <algorithm>

int filterMedian()
{
  int res = 0;

  std::sort(data, data + DATA_SIZE);

  if (DATA_SIZE % 2 == 0)
  {
      res = data[DATA_SIZE_H-1] + data[DATA_SIZE_H];
      res /=2;
  }
  else
  {
      res = data[DATA_SIZE_H];
  }
  return res;
}

#endif

#ifndef __FILTER_MEDIAN__
#define __FILTER_MEDIAN__

#include <algorithm>

int filterData()
{
  int res = 0;

  std::sort(data, data + SIZE);

  if (SIZE % 2 == 0)
  {
      res = data[SIZE/2-1] + data[SIZE/2];
      res /=2;
  }
  else
  {
      res = data[SIZE/2];
  }
  return res;
}
#endif

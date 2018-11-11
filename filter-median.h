#ifndef __DATA_MEDIAN__
#define __DATA_MEDIAN__

double filterData()
{
  double res = 0;

  std::sort(data, data + SIZE);

  double res = 0;
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

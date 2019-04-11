#ifndef __FILTER_EXP_SMOOTH__
#define __FILTER_EXP_SMOOTH__

static int filter_exp_smooth = -1;

int filterExpSmooth(int* dataPtr)
{
  double alp = 0.5;
  int last = dataPtr[DATA_SIZE-1];
  filter_exp_smooth = filter_exp_smooth >= 0
    ? last * alp + (1.0 - alp) * filter_exp_smooth
    : last;
  
  return filter_exp_smooth;
} 
#endif

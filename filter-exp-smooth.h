#ifndef __FILTER_EXP_SMOOTH__
#define __FILTER_EXP_SMOOTH__

static int filter_exp_smooth = -1;

int filterExpSmooth()
{
  double alp = 0.5;
  
  if (filter_exp_smooth >= 0)
  {
    int new_filtered_sample = data[current_sample-1] * alp + (1.0 - alp) * filter_exp_smooth;
    filter_exp_smooth = new_filtered_sample;  
  }  
  else
  {
    filter_exp_smooth = data[current_sample-1];
  }
  return filter_exp_smooth;
} 
#endif


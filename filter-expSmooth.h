#ifndef __FILTER_EXP_SMOOTH__
#define __FILTER_EXP_SMOOTH__

int filterExpSmooth()
{
  double alp = 0.5;
  
  if (prev_filtered_sample < 0)
  {
    prev_filtered_sample = data[current_sample-1];  
  }  
  else
  {
    int new_filtered_sample = data[current_sample-1] * alp + (1.0 - alp) * prev_filtered_sample;
    prev_filtered_sample = new_filtered_sample;
  }
  return prev_filtered_sample;
} 
#endif


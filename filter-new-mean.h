#ifndef __FILTER_NEW_MEAN__
#define __FILTER_NEW_MEAN__

int filterNewMean()
{
  double alp = 1.0;
  static int prev_filtered_sample = -1;
  if (prev_filtered_sample < 0)
  {
    prev_filtered_sample = data[current_sample-1];  
  }  
  else
  {
    int new_filtered_sample = prev_filtered_sample + alp * (data[current_sample-1] - prev_filtered_sample);
    prev_filtered_sample = new_filtered_sample;
  }
  return prev_filtered_sample;
} 
#endif


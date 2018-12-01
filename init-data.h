#ifndef __INIT_DATA__
#define __INIT_DATA__

#define DATA_SIZE 3u
#define MEDIAN_MID DATA_SIZE/2

int data[DATA_SIZE];
uint8_t current_sample = 0;
int prev_filtered_sample = -1;
#endif

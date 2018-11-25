#ifndef __INIT_DATA__
#define __INIT_DATA__


#define DATA_SIZE 9u
#define MEDIAN_WINDOW 3u
#define MEDIAN_MID MEDIAN_WINDOW/2
#define EXP_SMOOTH_WINDOW 1u
#define NEW_MEAN_WINDOW 1u

int data[DATA_SIZE];
uint8_t current_sample = 0;

#endif

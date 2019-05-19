#ifndef __INIT_DATA__
#define __INIT_DATA__

#define DATA_SIZE 9u

int dataConsumption[DATA_SIZE];
int dataCharging[DATA_SIZE];
int dataDischarge[DATA_SIZE];
int dataTemperature[DATA_SIZE];
int dataDepth[DATA_SIZE];

uint8_t current_sample = 0;

#endif

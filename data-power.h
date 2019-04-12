#ifndef __DATA_POWER__
#define __DATA_POWER__

#include "PowerMeter.h"

PowerMeter powerMeterConsumption(INA260::ADDR_GND,INA260::ADDR_GND);
/*
int readCurrent()
{
    return powerMeter.readCurrent();
}

int readVoltage()
{
  return powerMeter.readVoltage();
}

int readPower()
{
    return powerMeter.readPower();
}
*/
#endif /* CURRENT_SENSOR */

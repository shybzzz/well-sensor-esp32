#ifndef __DATA_POWER__
#define __DATA_POWER__

#include "PowerMeter.h"
PowerMeterConfig powerMeterConfigs[POWER_METERS_COUNT];
PowerMeter powerMeters[POWER_METERS_COUNT];

void setPowerMeterConfigs(PowerMeterConfig& configs, uint8_t addr, uint8_t mode, uint8_t ishct,
                          uint8_t vbusct, uint8_t avg, uint8_t rst)
{
  configs.addr = addr;
  configs.configReg.mode = mode;
  configs.configReg.ishct = ishct;
  configs.configReg.vbusct = vbusct;
  configs.configReg.avg = avg;
  configs.configReg.rst = rst;
}

#endif /* CURRENT_SENSOR */

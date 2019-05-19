#ifndef __POWER_METER__
#define __POWER_METER__

#include "INA260.h"

struct PowerMeterConfig
{
  INA260::ConfigurationRegister configReg;
  uint8_t addr;
};

class PowerMeter
{
  public:
        PowerMeter();
        bool init(PowerMeterConfig& configs);
        bool isInitialized() const;
        int readCurrent();
        int readVoltage();
        int readPower();
  
  private:
        INA260 sensor;
        uint8_t addr;
        double power;
        double voltage;
        double current;
        bool isInit;
};
#endif

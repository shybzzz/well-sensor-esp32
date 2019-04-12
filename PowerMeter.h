#ifndef __POWER_METER__
#define __POWER_METER__

#include "INA260.h"

class PowerMeter
{
  public:
        PowerMeter(INA260::AddressPin a0, INA260::AddressPin a1);
        bool init();
        bool isInitialized() const;
        int readCurrent();
        int readVoltage();
        int readPower();
  
  private:
        bool setInaConfig();
        INA260 sensor;
        double power;
        double voltage;
        double current;
        bool isInit;
};
#endif

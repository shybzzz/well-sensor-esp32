#ifndef __CURRENT_SENSOR__
#define __CURRENT_SENSOR__

#include "INA260.h"

struct PowerData{
  double current;
  double voltage;
  double watts;
};

/* Init power data structure pointer */
static PowerData *pwrDataPtr;
/* Set INA260 Address */
static INA260 ina260(INA260::ADDR_GND,INA260::ADDR_GND);

static bool setConfig()
{
  INA260::ConfigurationRegister configReg = {0};
  /* Average 64 samples for each reading */
  configReg.avg = INA260::AVG_64;
  configReg.vbusct = INA260::VBUSCT_1_1MS;
  configReg.ishct = INA260::ISHCT_1_1MS;
  configReg.mode = INA260::MODE_ISH_VBUS_CONTINUOUS;
  if(!ina260.writeConfigurationRegister(configReg))
  {
    return false;
  }

  return true;
}

bool currentSensorInit(PowerData *ptr)
{
  /* Init power data structure*/
  if (ptr != NULL)
  {
     pwrDataPtr = ptr;
     memset(pwrDataPtr, 0, sizeof(PowerData));
  }
  /* Call the begin() function to initialize the instance. This will also initialize the Wire/I2C library */
  ina260.begin();
  /* Set configuration */
  return setConfig();
}

bool getPowerData()
{
  if (pwrDataPtr != NULL)
  {
      /* Read current */
     if (!ina260.readCurrentRegisterInAmps(pwrDataPtr->current))
     {
        return false;
      }
     /* Read voltage */
     if (!ina260.readBusVoltageRegisterInVolts(pwrDataPtr->voltage))
     {
         return false;
     }
     /* Read power */
     if (!ina260.readPowerRegisterInWatts(pwrDataPtr->watts))
     {
         return false;
     }
     return true;
  }
  
  return false;
}

#endif /* CURRENT_SENSOR */

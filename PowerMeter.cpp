#include "PowerMeter.h"

PowerMeter::PowerMeter(INA260::AddressPin a0, INA260::AddressPin a1) : sensor(a0, a1), power(0), voltage(0), 
                                                                      current(0), isInit(false)
{
  
}

bool PowerMeter::init()
{
  /* Call the begin() function to initialize the instance. This will also initialize the Wire/I2C library */
  sensor.begin();
  /* Set configuration */
  isInit = setInaConfig();
  return isInit;
}

bool PowerMeter::isInitialized() const
{
  return isInit;
}
bool PowerMeter::setInaConfig()
{
    INA260::ConfigurationRegister configReg = {0};
    /* Average 64 samples for each reading */
    configReg.avg = INA260::AVG_64;
    configReg.vbusct = INA260::VBUSCT_1_1MS;
    configReg.ishct = INA260::ISHCT_1_1MS;
    configReg.mode = INA260::MODE_ISH_VBUS_CONTINUOUS;
    
    return sensor.writeConfigurationRegister(configReg);  
}

int PowerMeter::readCurrent()
{
    sensor.readCurrentRegisterInAmps(current);  
    return current * 1000;
}

int PowerMeter::readVoltage()
{
  sensor.readBusVoltageRegisterInVolts(voltage);
  return voltage * 1000;
}

int PowerMeter::readPower()
{
    sensor.readPowerRegisterInWatts(power);
    return power * 1000;
}

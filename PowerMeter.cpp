#include "PowerMeter.h"

PowerMeter::PowerMeter() : sensor(0), addr(0), power(0), 
                           voltage(0), current(0), isInit(false)
{
  
}

bool PowerMeter::init(PowerMeterConfig& configs)
{
  /* Call the begin() function to initialize the instance. This will also initialize the Wire/I2C library */
  addr = configs.addr;
  sensor = INA260(addr);
  
  sensor.begin();
  /* Set configuration */
  isInit = sensor.writeConfigurationRegister(configs.configReg); 
  return isInit;
}

bool PowerMeter::isInitialized() const
{
  return isInit;
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

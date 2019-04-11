#ifndef __DATA_POWER__
#define __DATA_POWER__

struct PowerData{
    double current;
    double voltage;
    double watts;
};

/* Set INA260 Address */
static INA260 ina260(INA260::ADDR_GND,INA260::ADDR_GND);

static bool setInaConfig()
{
    INA260::ConfigurationRegister configReg = {0};
    /* Average 64 samples for each reading */
    configReg.avg = INA260::AVG_64;
    configReg.vbusct = INA260::VBUSCT_1_1MS;
    configReg.ishct = INA260::ISHCT_1_1MS;
    configReg.mode = INA260::MODE_ISH_VBUS_CONTINUOUS;
    
    return ina260.writeConfigurationRegister(configReg);
}

bool InitCurrentSensor()
{
    /* Call the begin() function to initialize the instance. This will also initialize the Wire/I2C library */
    ina260.begin();
    /* Set configuration */
    return setInaConfig();
}

int readCurrent()
{
    double current = -1;
    ina260.readCurrentRegisterInAmps(current);  
    return current * 1000;
}

int readVoltage()
{
  double voltage = 0;
  ina260.readBusVoltageRegisterInVolts(voltage);
  return voltage * 1000;
}

int readPower()
{
    double power = -1;
    ina260.readPowerRegisterInWatts(power);
    return power * 1000;
}
#endif /* CURRENT_SENSOR */

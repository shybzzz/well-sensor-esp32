#ifndef ____
#define ____
setWifiConfig("ASUS_Guest", "11111111");
    setMqttConfig("m23.cloudmqtt.com", 12925, "tlwhlgqr", "g-VQc5c6w7eN", "Ihor");
 //   setPowerMeterConfigs(PowerMeterConfig& configs, uint8_t addr, uint8_t mode, uint8_t ishct,
 //                         uint8_t vbusct, uint8_t avg, uint8_t rst)

//set configs for INA with addr = (A1 = GND & A2 = GND)
    setPowerMeterConfigs(powerMeterConfigs[0], 0b10000000, INA260::MODE_ISH_VBUS_CONTINUOUS, INA260::ISHCT_1_1MS,
                          INA260::VBUSCT_1_1MS, INA260::AVG_64, 0);

    //set configs for INA with addr = (A1 = SDA & A2 = GND)
    setPowerMeterConfigs(powerMeterConfigs[1], 0b10010000, INA260::MODE_ISH_VBUS_CONTINUOUS, INA260::ISHCT_1_1MS,
                          INA260::VBUSCT_1_1MS, INA260::AVG_64, 0);
    
#endif

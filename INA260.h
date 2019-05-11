#ifndef INA260_H
#define INA260_H

#include <stdint.h>

class INA260 {
public:

  enum RegisterAddress {
    CONFIGURATION_REGISTER = 0x00,
    CURRENT_REGISTER = 0x01,
    BUS_VOLTAGE_REGISTER = 0x02,
    POWER_REGISTER = 0x03,
    MASK_ENABLE_REGISTER = 0x06,
    ALERT_LIMIT_REGISTER = 0x07,
    MANUFACTURER_ID_REGISTER = 0xFE,
    DIE_ID_REGISTER = 0xFF,
  };

  enum AveragingMode {
    AVG_1 = 0b000,
    AVG_4 = 0b001,
    AVG_16 = 0b010,
    AVG_64 = 0b011,
    AVG_128 = 0b100,
    AVG_256 = 0b101,
    AVG_512 = 0b110,
    AVG_1024 = 0b111,
  };

  enum VoltageConversionTime {
    VBUSCT_140US = 0b000,
    VBUSCT_204US = 0b001,
    VBUSCT_332US = 0b010,
    VBUSCT_588US = 0b011,
    VBUSCT_1_1MS = 0b100,
    VBUSCT_2_116MS = 0b101,
    VBUSCT_4_156MS = 0b110,
    VBUSCT_8_244MS = 0b0111,
  };

  enum CurrentConversionTime {
    ISHCT_140US = 0b000,
    ISHCT_204US = 0b001,
    ISHCT_332US = 0b010,
    ISHCT_588US = 0b011,
    ISHCT_1_1MS = 0b100,
    ISHCT_2_116MS = 0b101,
    ISHCT_4_156MS = 0b110,
    ISHCT_8_244MS = 0b111,
  };

  enum OperatingMode {
    MODE_POWER_DOWN1 = 0b000,
    MODE_ISH_TRIGGERED = 0b0001,
    MODE_VBUS_TRIGGERED = 0b010,
    MODE_ISH_VBUS_TRIGGERED = 0b011,
    MODE_POWER_DOWN2 = 0b0100,
    MODE_ISH_CONTINUOUS = 0b101,
    MODE_VBUS_CONTINUOUS = 0b110,
    MODE_ISH_VBUS_CONTINUOUS = 0b111,
  };

  enum AddressPin {
    ADDR_GND = 0b00,
    ADDR_VS = 0b01,
    ADDR_SDA = 0b10,
    ADDR_SCL = 0b11,
  };

  union ConfigurationRegister {
    struct __attribute__((packed)) {
      uint16_t mode : 3;
      uint16_t ishct : 3;
      uint16_t vbusct : 3;
      uint16_t avg : 3;
      uint16_t : 3;
      uint16_t rst : 1;
    };
    uint16_t rawValue;
  };

  union MaskEnableRegister {
    struct __attribute__((packed)) {
      uint16_t len : 1;
      uint16_t apol : 1;
      uint16_t ovf : 1;
      uint16_t cvrf : 1;
      uint16_t aff : 1;
      uint16_t : 5;
      uint16_t cnvr : 1;
      uint16_t pol : 1;
      uint16_t bul : 1;
      uint16_t bol : 1;
      uint16_t ucl : 1;
      uint16_t ocl : 1;
    };
    uint16_t rawValue;
  };

  union DieIdRegister {
    struct __attribute__((packed)) {
      uint16_t rid : 4;
      uint16_t did : 12;
    };
    uint16_t rawValue;
  };

  INA260(uint8_t addr = 0)
    : addr(0b1000000 | (addr & 0b1111)) {}

  INA260(AddressPin a0, AddressPin a1)
    : INA260((static_cast<uint8_t>(a1) << 2) | static_cast<uint8_t>(a0)) {}

  void begin();

  bool readRegister(uint8_t reg, uint16_t &value) const;
  bool writeRegister(uint8_t reg, uint16_t value) const;

  bool readConfigurationRegister(ConfigurationRegister &reg) const {
    return readRegister(CONFIGURATION_REGISTER, reg.rawValue);
  }

  bool writeConfigurationRegister(const ConfigurationRegister &reg) const {
    return writeRegister(CONFIGURATION_REGISTER, reg.rawValue);
  }

  bool readCurrentRegister(uint16_t &value) const {
    return readRegister(CURRENT_REGISTER, value);
  }

  bool readCurrentRegisterInAmps(double &value) const;

  bool readBusVoltageRegister(uint16_t &value) const {
    return readRegister(BUS_VOLTAGE_REGISTER, value);
  }

  bool readBusVoltageRegisterInVolts(double &value) const;

  bool readPowerRegister(uint16_t &value) const {
    return readRegister(POWER_REGISTER, value);
  }

  bool readPowerRegisterInWatts(double &value) const;

  bool readMaskEnableRegister(MaskEnableRegister &reg) const {
    return readRegister(MASK_ENABLE_REGISTER, reg.rawValue);
  }

  bool writeMaskEnableRegister(const MaskEnableRegister &reg) const {
    return writeRegister(MASK_ENABLE_REGISTER, reg.rawValue);
  }

  bool readAlertLimitRegister(uint16_t &value) const {
    return readRegister(ALERT_LIMIT_REGISTER, value);
  }

  bool readAlertLimitRegisterInAmps(double &value) const;

  bool readAlertLimitRegisterInVolts(double &value) const;

  bool readAlertLimitRegisterInWatts(double &value) const;

  bool writeAlertLimitRegister(uint16_t value) const {
    return writeRegister(ALERT_LIMIT_REGISTER, value);
  }

  bool writeAlertLimitRegisterInAmps(double value) const {
    return writeAlertLimitRegister(ampsToRaw(value));
  }

  bool writeAlertLimitRegisterInVolts(double value) const {
    return writeAlertLimitRegister(voltsToRaw(value));
  }

  bool writeAlertLimitRegisterInWatts(double value) const {
    return writeAlertLimitRegister(wattsToRaw(value));
  }

  bool readManufacturerIdRegister(char mfgStr[3]) const;

  bool readDieIdRegister(DieIdRegister &reg) const {
    return readRegister(DIE_ID_REGISTER, reg.rawValue);
  }

  bool reset() const;

  bool clearAlert() const;

  static double rawToAmps(int16_t value);

  static int16_t ampsToRaw(double value); 

  static double rawToVolts(uint16_t value); 

  static uint16_t voltsToRaw(double value);

  static double rawToWatts(uint16_t value);
 
  static uint16_t wattsToRaw(double value);

private:
  uint8_t addr;
};

#endif // INA260_H

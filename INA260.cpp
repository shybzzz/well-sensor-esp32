#include "INA260.h"
#include <Wire.h>

void INA260::begin() {
  static bool wireInitialized = false;
  if (! wireInitialized) {
    Wire.begin();
    wireInitialized = true;
  }
  reset();
}

bool INA260::readRegister(uint8_t reg, uint16_t &value) const {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(addr, 2u);
  if (Wire.available() == 2) {
    const uint16_t msb = Wire.read();
    const uint16_t lsb = Wire.read();
    value = (msb << 8) | lsb;
    return true;
  }
  return false;
}

bool INA260::writeRegister(uint8_t reg, uint16_t value) const {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  return (Wire.endTransmission() == 0);
}

bool INA260::readCurrentRegisterInAmps(double &value) const {
  uint16_t rawValue = 0;
  if (readCurrentRegister(rawValue)) {
    value = rawToAmps(static_cast<int16_t>(rawValue));
    return true;
  } else {
    return false;
  }
}

bool INA260::readBusVoltageRegisterInVolts(double &value) const {
  uint16_t rawValue = 0;
  if (readBusVoltageRegister(rawValue)) {
    value = rawToVolts(rawValue);
    return true;
  } else {
    return false;
  }
}
bool INA260::readPowerRegisterInWatts(double &value) const {
  uint16_t rawValue = 0;
  if (readPowerRegister(rawValue)) {
    value = rawToWatts(rawValue);
    return true;
  } else {
    return false;
  }
}

bool INA260::readAlertLimitRegisterInAmps(double &value) const {
  uint16_t val = 0;
  if (readAlertLimitRegister(val)) {
    value = rawToAmps(static_cast<int16_t>(val));
    return true;
  } else {
    return false;
  }
}

bool INA260::readAlertLimitRegisterInVolts(double &value) const {
  uint16_t val = 0;
  if (readAlertLimitRegister(val)) {
    value = rawToVolts(val);
    return true;
  } else {
    return false;
  }
}

bool INA260::readAlertLimitRegisterInWatts(double &value) const {
  uint16_t val = 0;
  if (readAlertLimitRegister(val)) {
    value = rawToWatts(val);
    return true;
  } else {
    return false;
  }
}

bool INA260::readManufacturerIdRegister(char mfgStr[3]) const {
  uint16_t value = 0;
  if (readRegister(MANUFACTURER_ID_REGISTER, value)) {
    mfgStr[0] = static_cast<char>((value >> 8) & 0xFF);
    mfgStr[1] = static_cast<char>(value & 0xFF);
    mfgStr[2] = '\0';
    return true;
  } else {
    return false;
  }
}

bool INA260::reset() const {
  ConfigurationRegister reg{};
  reg.rst = 1;
  return writeConfigurationRegister(reg);
}

bool INA260::clearAlert() const {
  MaskEnableRegister reg{};
  return readMaskEnableRegister(reg);
}

double INA260::rawToAmps(int16_t value) {
  return (value * 1.25) / 1000.0;
}

int16_t INA260::ampsToRaw(double value) {
  return (value * 1000.0) / 1.25;
}

double INA260::rawToVolts(uint16_t value) {
  return (value * 1.25) / 1000.0;
}

uint16_t INA260::voltsToRaw(double value) {
  return (value * 1000.0) / 1.25;
}

double INA260::rawToWatts(uint16_t value) {
  return (value * 10.0) / 1000.0;
}

uint16_t INA260::wattsToRaw(double value) {
  return (value * 1000.0) / 10.0;
}

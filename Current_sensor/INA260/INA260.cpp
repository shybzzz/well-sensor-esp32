
#include <INA260.h>
#include <Arduino.h>
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

#ifndef __DATA_DALLAS_TEMPERATURE__
#define __DATA_DALLAS_TEMPERATURE__

#include <DallasTemperature.h>

DallasTemperature dallasSensors(&ds);
DeviceAddress sensorAddress;

float getTemperature(DeviceAddress deviceAddress){
  float tempC = dallasSensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);
  return tempC;
}
 
void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void initDallasSensor()
{
  Serial.print("Search sensors...");
  dallasSensors.begin();
  Serial.print("Found: ");
  Serial.print(dallasSensors.getDeviceCount(), DEC);
  Serial.println(" sensors.");
 
  // find sensor:
  // find sensor addr by index.
  if (!dallasSensors.getAddress(sensorAddress, 0)) Serial.println("Can not find first sensor");
 
  Serial.print("Sensor addr: ");
  printAddress(sensorAddress);
  Serial.println();
 
  dallasSensors.setResolution(sensorAddress, 12);
}

int getDallasTempData()
{
  dallasSensors.requestTemperatures(); // ds18b20 calculates temrepature
  //  get temperature
  return getTemperature(sensorAddress);  
}
#endif

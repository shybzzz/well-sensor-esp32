#ifndef __DATA_DALLAS_TEMPERATURE__
#define __DATA_DALLAS_TEMPERATURE__

#include <DallasTemperature.h>

#define SENSOR_COUNT 2u
#define SENSOR_RESOLUTION 12u

DallasTemperature dallasSensors(&ds);
DeviceAddress sensorAddress[SENSOR_COUNT];

void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

float getTemperature(DeviceAddress deviceAddress){
  float tempC = dallasSensors.getTempC(deviceAddress);
  printAddress(deviceAddress);
  Serial.print("Temp C: ");
  Serial.println(tempC);
  return tempC;
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
  for (uint8_t i = 0; i < SENSOR_COUNT; ++i) {
    
    if (!dallasSensors.getAddress(sensorAddress[i], i)) {
   
      Serial.print("Can not find sensor #");
      Serial.println(i); 
    }
    Serial.print("Sensor addr: ");
    printAddress(sensorAddress[i]);
    Serial.println();

    dallasSensors.setResolution(sensorAddress[i], SENSOR_RESOLUTION);
  }
}

int getDallasTempData()
{
  dallasSensors.requestTemperatures(); // ds18b20 calculates temrepature
  //  get temperature
  return getTemperature(sensorAddress[1]) - getTemperature(sensorAddress[0]);  
}
#endif

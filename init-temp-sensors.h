#ifndef __INIT_TEMPERATURE_SENSORS__
#define __DATA_TEMPERATURE_SENSORS__

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_PIN      32u
#define TO_CELSIUS    0.0625
#define RAW_DATA_SIZE 2u

#define SENSOR_COUNT 2u
#define SENSOR_RESOLUTION 12u
#define NO_CONNECTED_SENSORS -10000

OneWire ds(TEMP_PIN);
DallasTemperature dallasSensors(&ds);
DeviceAddress sensorAddress[SENSOR_COUNT];
uint8_t connectedSensors = 0;

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
  connectedSensors = dallasSensors.getDeviceCount();
  Serial.print(connectedSensors, DEC);
  Serial.println(" sensors.");
 
  // find sensor:
  // find sensor addr by index.
  for (uint8_t i = 0; i < connectedSensors; ++i) 
  {
    
    if (!dallasSensors.getAddress(sensorAddress[i], i)) 
    {
   
      Serial.print("Can not find sensor #");
      Serial.println(i); 
    }
    Serial.print("Sensor addr: ");
    printAddress(sensorAddress[i]);
    Serial.println();

    dallasSensors.setResolution(sensorAddress[i], SENSOR_RESOLUTION);
  }
}


#endif

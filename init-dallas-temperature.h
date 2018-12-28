#ifndef __INIT_DALLAS_TEMPERATURE__
#define __INIT_DALLAS_TEMPERATURE__

#include <OneWire.h>
#include <DallasTemperature.h>

#define DALLAS_TEMPERATURE_PIN      32u
#define TO_CELSIUS    0.0625
#define DALLAS_TEMPERATURE_RAW_DATA_SIZE 2u

#define DALLAS_TEMPERATURE_SENSOR_RESOLUTION 12u

#define MAX_DALLAS_TEMPERATURE_SENSORS_COUNT 2u

OneWire ds(DALLAS_TEMPERATURE_PIN);
DallasTemperature dallasSensors(&ds);
DeviceAddress sensorAddress[MAX_DALLAS_TEMPERATURE_SENSORS_COUNT];
uint8_t connectedSensors = 0;

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


void initDallasSensor()
{
  dallasSensors.begin();
  connectedSensors = dallasSensors.getDeviceCount();

  Serial.print("Search sensors...");
  Serial.print("Found: ");
  Serial.print(connectedSensors, DEC);
  Serial.println(" sensors.");

  // find sensor:
  // find sensor addr by index.
  for (uint8_t i = 0; i < connectedSensors || i < MAX_DALLAS_TEMPERATURE_SENSORS_COUNT; ++i)
  {
    if (dallasSensors.getAddress(sensorAddress[i], i)) {
      dallasSensors.setResolution(sensorAddress[i], DALLAS_TEMPERATURE_SENSOR_RESOLUTION);
      Serial.print("Sensor addr: ");
      printAddress(sensorAddress[i]);
      Serial.println();
    } else {
      Serial.print("Can not find sensor #");
      Serial.println(i);
    }
  }
}


#endif

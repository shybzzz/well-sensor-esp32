#ifndef __DATA_DALLAS_TEMPERATURE__
#define __DATA_DALLAS_TEMPERATURE__

int getDallasTempData()
{
  dallasSensors.requestTemperatures(); // ds18b20 calculates temrepature
  //  get current temperature
  return 100 * (connectedSensors == 0 ? 0 :
                connectedSensors == 1 ? dallasSensors.getTempC(sensorAddress[0]) :
                dallasSensors.getTempC(sensorAddress[1]) - dallasSensors.getTempC(sensorAddress[0]));
}
#endif

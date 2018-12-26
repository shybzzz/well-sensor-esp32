#ifndef __DATA_DALLAS_TEMPERATURE__
#define __DATA_DALLAS_TEMPERATURE__

int getDallasTempData()
{
  dallasSensors.requestTemperatures(); // ds18b20 calculates temrepature
  //  get temperature
  int result = 0;
  if (connectedSensors == 0)
  {
    result = NO_CONNECTED_SENSORS;
  }
  else if (connectedSensors == 1)
  {
    result = dallasSensors.getTempC(sensorAddress[0]);
  }
  else 
  {
    result = dallasSensors.getTempC(sensorAddress[1]) - dallasSensors.getTempC(sensorAddress[0]);    
  }
  return result;
}
#endif

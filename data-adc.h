#ifndef __ROUTINE_ADC__
#define __ROUTINE_ADC__

#define SENSOR_PIN 36

int getADC_Data()
{
  return analogRead(SENSOR_PIN);  
}
#endif

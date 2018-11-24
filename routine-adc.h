#ifndef __ROUTINE_ADC__
#define __ROUTINE_ADC__

int getADC_Data()
{
  return analogRead(SENSOR_PIN);  
}
#endif

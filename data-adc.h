#ifndef __ROUTINE_ADC__
#define __ROUTINE_ADC__

int getADC_Data()
{
  return analogRead(ADC_PIN);  
}
#endif

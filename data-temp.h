#ifndef __DATA_TEMPERATURE__
#define __DATA_TEMPERATURE__

#include <OneWire.h>

#define TEMP_PIN 32u

OneWire ds(TEMP_PIN);

int getTempData()
{
  byte data[2];
   
  ds.reset();
  ds.write(0xCC); 
  ds.write(0x44); 
   
  delay(1000);   
   
  ds.reset(); 
  ds.write(0xCC); 
  ds.write(0xBE); 
 
  data[0] = ds.read(); 
  data[1] = ds.read(); 
 
  float temperature =  ((data[1] << 8) | data[0]) * 0.0625;
  return temperature;
}
#endif

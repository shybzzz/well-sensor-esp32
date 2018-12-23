#ifndef __DATA_TEMPERATURE__
#define __DATA_TEMPERATURE__

#include <OneWire.h>

#define TEMP_PIN      32u
#define TO_CELSIUS    0.0625
#define RAW_DATA_SIZE 2u
OneWire ds(TEMP_PIN);

int getTempData()
{
  byte data[RAW_DATA_SIZE];
   
  ds.reset();
  ds.write(0xCC); 
  ds.write(0x44); 
   
  delay(1000);   
   
  ds.reset(); 
  ds.write(0xCC); 
  ds.write(0xBE); 
 
  data[0] = ds.read(); 
  data[1] = ds.read(); 
 
  float temperature =  ((data[1] << 8) | data[0]) * TO_CELSIUS;
  return temperature;
}
#endif

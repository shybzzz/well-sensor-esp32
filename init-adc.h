#ifndef __INIT_ADC__
#define __INIT_ADC__

#define ADC_PIN 36u
struct ADC_Config
{
  uint8_t resolution = 11;
  uint8_t cycles = 8;
  uint8_t samples = 1;
  uint8_t clockDiv = 1;
  adc_attenuation_t attenuation = ADC_0db;  
};
ADC_Config adc_config;

void initADC()
{
  analogReadResolution(adc_config.resolution);
  analogSetCycles(adc_config.cycles);
  analogSetSamples(adc_config.samples);
  analogSetClockDiv(adc_config.clockDiv);  
  analogSetAttenuation(adc_config.attenuation);
}
#endif

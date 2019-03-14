#include "current_sensor.h"

/* Init power data structure */
static PowerData pwrData;

void setup() 
{
  Serial.begin(115200);
  /* Init current sensor */
  if (!currentSensorInit(&pwrData))
  {
    Serial.println("Not Initialized");
  }
}

void loop() 
{
  /* Read data from sensor */
  if (getPowerData())
  {
     Serial.print("Current: ");
     Serial.println(pwrData.current, 5);
     Serial.print("Voltage: ");
     Serial.println(pwrData.voltage, 5);
     Serial.print("Power: ");
     Serial.println(pwrData.watts, 5);
  }
  else
  {
    Serial.println("NODATA - Connection Error!!!!");
  }
  delay(1000);
}

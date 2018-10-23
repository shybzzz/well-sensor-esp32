portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
bool buttonClicked = false;
void IRAM_ATTR handleIntr()
{
  Serial.println("Resetting Well Sensor...");
  portENTER_CRITICAL_ISR(&mux);
  portEXIT_CRITICAL_ISR(&mux);  
  buttonClicked = true;
}

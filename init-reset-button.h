#ifndef __INIT_RESET_BUTTON__
#define __INIT_RESET_BUTTON__

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
bool buttonClicked = false;
void IRAM_ATTR handleIntr()
{
  Serial.println("Resetting Well Sensor...");
  portENTER_CRITICAL_ISR(&mux);
  portEXIT_CRITICAL_ISR(&mux);  
  buttonClicked = true;
}

void formatFlash(){
  
  Serial.println("Removing Stored Config...");
  SPIFFS.format();
  
  Serial.println("Restarting Well Sensor...");
  delay(1000);
  ESP.restart();  
  
}

void initResetButton() {
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleIntr, HIGH);
}

#endif

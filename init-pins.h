#ifndef __INIT_PINS__
#define __INIT_PINS__

#define LED_RED 12u
#define LED_GREEN 14u
#define LED_BLUE 27u
#define BTN_PIN 35u

void initPins() {
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  pinMode(BTN_PIN, INPUT_PULLDOWN);
  
}

void redLight() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void dance(uint16_t d) { 
   
  digitalWrite(LED_RED, HIGH);
  delay(d);
  
  digitalWrite(LED_BLUE, LOW);
  delay(d);
  
  digitalWrite(LED_GREEN, HIGH);
  delay(d);
  
  digitalWrite(LED_RED, LOW);
  delay(d);
  
  digitalWrite(LED_BLUE, HIGH);
  delay(d);
  
  digitalWrite(LED_GREEN, LOW);
  delay(d);  
  
}

#endif

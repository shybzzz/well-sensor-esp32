#ifndef __INIT_PINS__
#define __INIT_PINS__

#define LED_RED 12u
#define LED_GREEN 14u
#define LED_BLUE 27u

void initLed() {
  
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
    
}

void redLight() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);  
}

void yellowLight() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);  
}

void greenLight() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);  
}

void blueLight() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);  
}

void violetLight() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);  
}

void whiteLight() {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);  
}

void noLight() {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);  
}

void dance(uint16_t d) { 
   
  noLight();
  delay(d);
  redLight();
  delay(d);
  yellowLight();
  delay(d);
  greenLight();
  delay(d);
  blueLight();
  delay(d);
  violetLight();
  delay(d);
  
}

#endif

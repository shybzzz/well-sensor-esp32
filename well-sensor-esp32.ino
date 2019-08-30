#include "Arduino.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ArduinoJson.h"
#include "PubSubClient.h"
#include "esp32-hal-adc.h"
#include "INA260.h"

#include "definitions.h"

#include "init-led.h"
#include "init-button.h"
#include "init-qr.h"
#include "init-spiffs.h"
#include "init-data.h"
#include "init-esp32.h"
#include "init-wifi.h"
#include "init-mqtt.h"
#include "init-dallas-temperature.h"
#include "init-adc.h"
#include "data-median.h"
#include "data-adc.h"
#include "data-dallas-temperature.h"
#include "data-power.h"
#include "PowerMeter.h"

#include "filter-median.h"
#include "filter-exp-smooth.h"
#include "filter-mean.h"

#include "init-combined-sensors.h"
#include "init-server.h"


void setup() {

  Serial.begin(115200);
  Serial.println("Starting Well Sensor...");

  initLed();
  initButton();
  initQr();
 
  if (initSPIFSS()) {
    
    //set configs for INA with addr = (A1 = GND & A2 = GND)
    setPowerMeterConfigs(powerMeterConfigs[0], 0b1000000, INA260::MODE_ISH_VBUS_CONTINUOUS, INA260::ISHCT_1_1MS,
                          INA260::VBUSCT_1_1MS, INA260::AVG_64, 0);

    //set configs for INA with addr = (A1 = SDA & A2 = GND)
    setPowerMeterConfigs(powerMeterConfigs[1], 0b1001000, INA260::MODE_ISH_VBUS_CONTINUOUS, INA260::ISHCT_1_1MS,
                          INA260::VBUSCT_1_1MS, INA260::AVG_64, 0);
                      
    setWifiConfig("ASUS_Guest", "11111111");
    setMqttConfig("m23.cloudmqtt.com", 12925, "tlwhlgqr", "g-VQc5c6w7eN", "Ihor");

                          
    initWifi();
    initMqtt();
    initSensors();    
    initEsp32();
  }

  Serial.println("Well Sensor is running");
}

void loop() {

  if (buttonClicked) {
    isAPRunning
    ? stopAP()
    : startAP();
    buttonClicked = false;
    return;
  }

  listenServer();

  if (isAPRunning) {
    whiteLight();
    return;
  }

  if (!isWifiConfigSet) {
    redLight();
    return;
  }

  if (!isWifiConnected()) {
    yellowLight();
    reconnectWifi();
    return;
  }

  if (!isMqttConfigSet) {
    greenLight();
    return;
  }

  if (!mqttClient.connected()) {
    blueLight();
    reconnectMqtt();
    return;
  }

  noLight();

  if (!measure()) {
    dance(10);
  }

  mqttClient.loop();
}

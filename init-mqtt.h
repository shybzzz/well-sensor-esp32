#ifndef __INIT_MQTT__
#define __INIT_MQTT__

#include "definitions.h"

struct MqttConfig
{
  char server[MAX_STR_LEN];
  int port;
  char user[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
};
WiFiClient espClient;
PubSubClient mqttClient(espClient);
#endif

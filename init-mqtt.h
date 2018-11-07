#ifndef __INIT_MQTT__
#define __INIT_MQTT__

struct MqttConfig
{
  char server[MAX_STR_LEN];
  int port;
  char user[MAX_STR_LEN];
  char pwd[MAX_STR_LEN];
};

MqttConfig mqttConfig;
const char *mqttFileName = "/mqttConfig.json";
bool isMqttConfigSet = false;

WiFiClient espClient;
PubSubClient mqttClient(espClient);


void setMqttConfig(const char* server, int port, const char* user, const char* pwd){
  
  memset(mqttConfig.server, 0, MAX_STR_LEN);
  memcpy(mqttConfig.server, server, MAX_STR_LEN);

  mqttConfig.port = port;

  memset(mqttConfig.user, 0, MAX_STR_LEN);
  memcpy(mqttConfig.user, user, MAX_STR_LEN);

  memset(mqttConfig.pwd, 0, MAX_STR_LEN);
  memcpy(mqttConfig.pwd, pwd, MAX_STR_LEN);

  isMqttConfigSet = true;
}

bool loadMqttConfig(){
  
  bool res = readFile(mqttFileName);

  if (res) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(fileBuff);
      res = json.success();
      if (res) {
        setMqttConfig(json[MQTT_CONFIG_SERVER], json[MQTT_CONFIG_PORT], 
                      json[MQTT_CONFIG_USER], json[MQTT_CONFIG_PWD]);
        res = true; 
      } else {
        Serial.println("Mqtt configuration is corrupted");  
      }
  }
  
  return res;
}

bool saveMqttConfig(const char* server, int port, const char* user, const char* pwd) {
  
  bool res = false;  

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[MQTT_CONFIG_SERVER] = server;
  json[MQTT_CONFIG_PORT] = port;
  json[MQTT_CONFIG_USER] = user;
  json[MQTT_CONFIG_PWD] = pwd;

  res = saveJson(mqttFileName, json);

  if (res) {
    Serial.println("Mqtt configs are saved.");  
  }
  return res;
}

void initMqtt() {

  //tbd: set mqtt callback
  loadMqttConfig();
  if (isMqttConfigSet){
      mqttClient.setServer(mqttConfig.server, mqttConfig.port);
      Serial.println("Connecting to mqtt server...");  
      mqttClient.connect(qrConfig.DEVICE_ID, mqttConfig.user, mqttConfig.pwd));
  }
}
#endif

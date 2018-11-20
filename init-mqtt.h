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

void mqttCallback(char* topic, byte* payload, size_t len){
  
  Serial.print("Message arrived: [");
  Serial.print(topic);
  Serial.print("] =");
  for (size_t i = 0; i < len; ++i)
  {
    Serial.print(char(payload[i]));  
  }
  Serial.println();  
}
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

bool containsMqttConfig(JsonObject& json) {
  return
    json.containsKey(MQTT_CONFIG_SERVER)
    && json.containsKey(MQTT_CONFIG_PORT)
    && json.containsKey(MQTT_CONFIG_USER)
    && json.containsKey(MQTT_CONFIG_PWD);
}

bool loadMqttConfig() {
  
  bool res = false;

  if (readFile(mqttFileName)) {
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(fileBuff);
      if (
        json.success()
        && containsMqttConfig(json)
      ) {
        setMqttConfig(json[MQTT_CONFIG_SERVER], json[MQTT_CONFIG_PORT], 
                      json[MQTT_CONFIG_USER], json[MQTT_CONFIG_PWD]);
        res = true; 
      } else {
        Serial.println("Mqtt configuration is corrupted");  
      }
  }
  
  return res;
}

bool saveMqttConfigToSPIFFS(const char* server, int port, const char* user, const char* pwd) {
  
  bool res = false;  

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  json[MQTT_CONFIG_SERVER] = server;
  json[MQTT_CONFIG_PORT] = port;
  json[MQTT_CONFIG_USER] = user;
  json[MQTT_CONFIG_PWD] = pwd;

  if (saveJson(mqttFileName, json)) {
    res = true;
    Serial.println("Mqtt configs are saved.");  
  }
  return res;
}

bool connectMqtt(const char* user, const char* pwd){
  bool res = false;
      
  mqttClient.connect(qrConfig.DEVICE_ID, user, pwd);
  if (mqttClient.connected()) {    
    res = true;
    Serial.print("Connected to mqtt broker");    
  } else {
    Serial.print("failed, rc = ");
    Serial.println(mqttClient.state());
  }

  return res;
}

bool reconnectMqtt() {
  return connectMqtt(mqttConfig.user, mqttConfig.pwd);
}

bool setMqttServer(const char* server, int port){
  bool res = false;
  if (isMqttConfigSet) {
    mqttClient.setServer(mqttConfig.server, mqttConfig.port);    
    res = true;
    Serial.print("Mqtt server: ");  
    Serial.print(server);
    Serial.print(":");
    Serial.println(port);
  }
  return res;
 
}

void initMqtt() {
  mqttClient.setCallback(mqttCallback);  
  loadMqttConfig();
  setMqttServer(mqttConfig.server, mqttConfig.port);
}

void publishInt(const char* topic, int d) {
  char payload[5];
  sprintf(payload, "%d", d);
  mqttClient.publish(topic, payload);
}

bool tryConnectMqtt(const char* server, int port, const char* user, const char* pwd){
  return 
    setMqttServer(server, port)
    && connectMqtt(user, pwd);
}

uint8_t saveMqttConfig(JsonObject& json) {
  uint8_t res = 0;
  
  if(
      json.success()
      && containsMqttConfig(json)
  ) {
        
    const char* server = json[MQTT_CONFIG_SERVER];
    int port = json[MQTT_CONFIG_PORT];
    const char* user = json[MQTT_CONFIG_USER];
    const char* pwd = json[MQTT_CONFIG_PWD];
    
    if(tryConnectMqtt(server, port, user, pwd)) {
      if(saveMqttConfigToSPIFFS(server, port, user, pwd)) {
        setMqttConfig(server, port, user, pwd);
        Serial.println("MQTT is configured");
      } else {
        res = SAVE_MQTT_FAILED_RESPONSE_HEADER;
        Serial.println("Saving mqttConfig failed");                
      }
    } else {
      res = MQTT_CONNECTION_FAILED_RESPONSE_HEADER;
      Serial.println("Error. Could not connect to MQTT with mqttConfig provided");
    }
  } else {
    res = INVALID_MQTT_CONFIG_RESPONSE_HEADER;
    Serial.println("Error. Invalid wifiConfig is received");
  }
  
  return res;
}

#endif

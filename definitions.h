#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#define MAX_STR_LEN                                 64u
#define WIFI_TIMEOUT                                8u
#define MQTT_TIMEOUT                                1u

// TCP Request headers
#define SET_WIFI_CONFIG_REQUEST_HEADER              1u
#define GET_IP_REQUEST_HEADER                       2u
#define SET_CONFIG_REQUEST_HEADER                   3u

// TCP Response headers
#define INVALID_WIFI_CONFIG_RESPONSE_HEADER         1u
#define WIFI_CONNECTION_FAILED_RESPONSE_HEADER      2u
#define SAVE_WIFI_FAILED_RESPONSE_HEADER            3u
#define SAVE_MQTT_FAILED_RESPONSE_HEADER            4u
#define MQTT_CONNECTION_FAILED_RESPONSE_HEADER      5u
#define INVALID_MQTT_CONFIG_RESPONSE_HEADER         6u

// WIFI Config Keys
#define WIFI_CONFIG_IP                              ("ip")
#define WIFI_CONFIG_SSID                            ("ssid")
#define WIFI_CONFIG_PWD                             ("wifiPwd")

// MQTT Config Keys
#define MQTT_CONFIG_SERVER                          ("server")
#define MQTT_CONFIG_PORT                            ("port")
#define MQTT_CONFIG_USER                            ("user")
#define MQTT_CONFIG_PWD                             ("mqttPwd")
#define MQTT_CONFIG_DEVICE_ID                       ("deviceId")

// MQTT Payload Keys
#define PAYLOAD_DEVICE                              ("device")
#define PAYLOAD_TOPIC                               ("topic")
#define PAYLOAD_VALUE                               ("value")
#define PAYLOAD_VALUE_DATA                          ("data")
#define PAYLOAD_VALUE_MEDIAN                        ("median")
#define PAYLOAD_VALUE_MEAN                          ("mean")
#define PAYLOAD_VALUE_EXP_SMOOTH                    ("expSmooth")

#define PAYLOAD_INA260_CONSUMPTION                  ("consumption")
#define PAYLOAD_INA260_CHARGING                     ("solarCurrent")
#define PAYLOAD_DS18B20                             ("temperature")
#define PAYLOAD_GUT800                              ("depth")
#define PAYLOAD_DISCHARGE                           ("discharge")

// MQTT Topics
#define TOPIC_DATA                                  ("Data")
#define TOPIC_SEPARATOR                             ("/")

// Sensor Config Keys
#define SENSOR_CONFIG_TYPE                          ("sensorType")
#define POWER_ADDR                                  ("addr")
#define POWER_MODE                                  ("mode")
#define POWER_ISHCT                                 ("ishct")
#define POWER_VBUSCT                                ("vbusct")
#define POWER_AVG                                   ("avg")  
#define POWER_RST                                   ("rst")

// Sensor Types
#define SENSOR_SIMULATED                            0u
#define SENSOR_ANALOG_TEST                          1u
#define SENSOR_DS18B20                              2u
#define SENSOR_GUT800                               3u
#define SENSOR_INA260_VOLTAGE                       4u
#define SENSOR_INA260_CURRENT                       5u
#define SENSOR_INA260_POWER                         6u
#define SENSOR_COMBINED                             7u

#define POWER_METERS_COUNT                          2u

// Esp related configs
#define MEASUREMENT_DELAY                           ("delay")
#endif

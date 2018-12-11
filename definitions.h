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

// MQTT Payload Keys
#define PAYLOAD_DEVICE                              ("device")
#define PAYLOAD_TOPIC                               ("topic")
#define PAYLOAD_VALUE                               ("value")

// MQTT Topics
#define TOPIC_DATA                                  ("Data")
#define TOPIC_FILTER_MEAN                           ("Filter/Mean")
#define TOPIC_FILTER_MEDIAN                         ("Filter/Median")
#define TOPIC_FILTER_EXP_SMOOTH                     ("Filter/ExpSmooth")

#endif

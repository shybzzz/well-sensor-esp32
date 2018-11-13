#ifndef __DEFINITIONS__
#define __DEFINITIONS__

//Sizes

#define MAX_STR_LEN 64u

//WIFI

#define WIFI_TIMEOUT 8u

#define SET_WIFI_CONFIG_REQUEST_HEADER '1'
#define GET_IP_REQUEST_HEADER '2'

#define INVALID_WIFI_CONFIG_RESPONSE_HEADER '1'
#define WIFI_CONNECTION_FAILED_RESPONSE_HEADER '2'

#define WIFI_CONFIG_SSID ("ssid")
#define WIFI_CONFIG_PWD ("pwd")

//MQTT

#define MQTT_CONFIG_SERVER ("server")
#define MQTT_CONFIG_PORT ("port")
#define MQTT_CONFIG_USER ("user")
#define MQTT_CONFIG_PWD ("pwd")
#endif

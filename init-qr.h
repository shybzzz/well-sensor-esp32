#ifndef __INIT_QR__
#define __INIT_QR__

struct QRConfig {
  char AP_SSID[MAX_STR_LEN];
  char AP_PWD[MAX_STR_LEN];
  char DEVICE_ID[MAX_STR_LEN];
};

QRConfig qrConfig;

void initQr() {
  char ssid[] = "Well Sensor";
  char pwd[] = "11111111";
  char device_id[] = "ESP_Client_Ihor";
  strncpy(qrConfig.AP_SSID, ssid, strlen(ssid));
  strncpy(qrConfig.AP_PWD, pwd, strlen(pwd)); 
  strncpy(qrConfig.DEVICE_ID, device_id, strlen(device_id));  
}

#endif

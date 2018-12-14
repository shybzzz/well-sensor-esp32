#ifndef __INIT_QR__
#define __INIT_QR__

struct QRConfig {
  char AP_SSID[MAX_STR_LEN];
  char AP_PWD[MAX_STR_LEN];
};

QRConfig qrConfig;

void initQr() {
  char ssid[] = "Well Sensor";
  char pwd[] = "11111111";
  strncpy(qrConfig.AP_SSID, ssid, strlen(ssid));
  strncpy(qrConfig.AP_PWD, pwd, strlen(pwd));   
}

#endif

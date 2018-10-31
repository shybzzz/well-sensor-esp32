#ifndef __INIT_QR__
#define __INIT_QR__

struct QRConfig {
  char AP_SSID[64];
  char AP_PWD[64];
};

QRConfig qrConfig;

void initQr() {
  strncpy(qrConfig.AP_SSID, "Well Sensor", sizeof(qrConfig.AP_SSID));
  strncpy(qrConfig.AP_PWD, "11111111", sizeof(qrConfig.AP_PWD));    
}

#endif

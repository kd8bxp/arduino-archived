#ifndef __TRIDENTTD_ESPRANDORM_H__
#define __TRIDENTTD_ESPRANDORM_H__

#include <Arduino.h>
#if defined ESP8266
#include <osapi.h>
#endif

#if defined ESP8266 || defined ESP32
class TridentTD_ESPRandom {
  public:
    TridentTD_ESPRandom() {}
    uint32_t random();
    int      random(uint32_t to_num);
    int      random(uint32_t from_num, uint32_t to_num);

    String   genUUID(size_t len=16);
    String   genMacAddress();
};

extern TridentTD_ESPRandom esp;

#endif  // defined ESP8266 ESP32
#endif  // __TRIDENTTD_ESPRANDORM_H__

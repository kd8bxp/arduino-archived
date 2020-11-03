#include "TridentTD_ESP_TrueRandom.h"
#include <algorithm>


#if defined ESP8266 || defined ESP32
#if defined ESP8266
#define MAGIC_NUM  0x3FF20E44L
#endif

#if defined ESP32
#define MAGIC_NUM  0x3FF75144
#endif

uint32_t TridentTD_ESPRandom::random(){
  return *(uint32_t*) MAGIC_NUM;
}

int TridentTD_ESPRandom::random(uint32_t to_num){
  if(to_num == 0) return this->random();  
  //return  ((to_num)? 1:-1)* this->random() % abs(to_num);
  return  ((to_num)? 1:-1)* this->random()%(abs((int)to_num));
}

int TridentTD_ESPRandom::random(uint32_t from_num, uint32_t to_num){
  if(from_num == to_num) return 0;
  if(from_num > to_num ) std::swap(from_num, to_num);
  return  from_num + this->random()%(to_num-from_num);
}

String TridentTD_ESPRandom::genUUID(size_t len){
  String _uuid = "";
  for(int i=0; i< len; ++i) _uuid += String( this->random(0xFF), HEX)+ String((i<len-1)?":": "");
  return _uuid;
}

String TridentTD_ESPRandom::genMacAddress() {
  return   genUUID(6);
}

TridentTD_ESPRandom esp;

#endif

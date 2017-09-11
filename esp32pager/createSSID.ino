

void createSSID() {
  
  uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  uint16_t chip = (uint16_t)(chipid>>32);
  snprintf(ssid,15,"LoRaHam-%04X",chip);
  
}


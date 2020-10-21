void radioon(){
 LoRa.setPins(SS,RST,DI0);
  
  if (!LoRa.begin(BAND)) {
    serialPrt("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc(); //encable CRC checking - off by default
  LoRa.setSpreadingFactor(loraSpreadingFactor);
  LoRa.setCodingRate4(loraCodingRate);
  LoRa.setSignalBandwidth(loraBandwidth);
 
}

void radiooff(){
  //arduino-LoRa library provides a sleep, and a idle mode (Sleep is what I beleive we need here) 
  //I'm not sure this is needed once deep sleep was turned on. More
  //Information is needed.
  LoRa.sleep();
  delay(10);
}

void startGPS() {
   if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    serialPrt("AXP192 Begin PASS");
  } else {
    serialPrt("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  GPS.begin(9600, SERIAL_8N1, 34, 12);   //17-TX 18-RX
}

void getGPS() {
  while (GPS.available()) {
      gps.encode(GPS.read());
   }
}

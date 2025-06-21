void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("");
  Serial.println("Check ID in:");
  Serial.println("https://www.wemos.cc/verify_products");
  Serial.printf("Chip ID = %08X\n", ESP.getChipId());
  Serial.println(ESP.getChipId());
  Serial.println("");
  Serial.println("");
  uint32_t chipid=ESP.getChipId();
   
    char clientid[25];
    snprintf(clientid,25,"WIFI-Display-%08X",chipid);
    Serial.print("Client ID: ");
    Serial.println(clientid);
  delay(5000);
}

void loop() {
  
}

//found at:
// https://techtutorialsx.com/2017/04/26/esp8266-flash-chip-information-functions/

void setup() {
 
  Serial.begin(115200);
  delay(1000);
  Serial.println();
 
  Serial.print("Chip ID: ");
  Serial.println(ESP.getFlashChipId());
 
  Serial.print("Chip Real Size: ");
  Serial.println(ESP.getFlashChipRealSize());
 
  Serial.print("Chip Size: ");
  Serial.println(ESP.getFlashChipSize());
 
  Serial.print("Chip Speed: ");
  Serial.println(ESP.getFlashChipSpeed());
 
  Serial.print("Chip Mode: ");
  Serial.println(ESP.getFlashChipMode());
}
 
void loop() {
}

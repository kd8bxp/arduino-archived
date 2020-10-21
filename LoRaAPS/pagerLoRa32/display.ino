
void displaysetup() {
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  oled.setFont(Adafruit5x7);
  oled.clear();
}


void wordWrap(String sr) {
  //This works with the Adafruit5x7 font, it should work with any font that is 5x7
 oled.println("");
 oled.print("Message: ");
  int charCount = 9;
  int sizeWord = sr.length()+1; //sizeof(sr);
  char buf[sizeWord]; //sizeof(sr)];
  sr.toCharArray(buf, sizeof(buf));
  char *p = buf;
  char *str;
    while ((str = strtok_r(p, " ", &p)) != NULL) { // delimiter is the space
      charCount += strlen(str) + 1;
    if (charCount >= 20) {  charCount = strlen(str) + 1; oled.print("\n"); } 
      
        oled.print(str); oled.print(" "); 
      }
 }

 void displayMsg(String ft, String mt) {
  oled.clear();
  oled.println("\nFrom: " + ft);
  serialPrt("From: " +ft);
  serialPrt(mt);
  wordWrap(mt);
  
 }

void displayCall() {
  
        oled.home();
        oled.println("");
        oled.print("My Call: ");
        oled.println(CALLSIGN);
        
}

void displayPath() {
  oled.clear();
  int count = 0;
  oled.println("\nFrom : " + from);
  for (int i=1;i<4;i++){
    if (path[i].indexOf("NOCALL") >= 0) { } else {
    oled.print("Hop "); oled.print(i); oled.println(": " + path[i]);
    count++;
    } 
  }
  
  if (count == 0) {oled.println("Direct Connection"); } else {oled.print("Total Hops: "); oled.println(count); }
  oled.println("");
  oled.println("RSSI: " + rssi);
}

    /**
    * Code to display the time & date from a GPS receiver on a LCD.
    *
    * This code was inspired by http://arduino.cc/en/Tutorial/LiquidCrystal and
    * http://playground.arduino.cc/Tutorials/GPS
    *
    * For more information, see http://quaxio.com/arduino_gps/
    */
     
    #include <LiquidCrystal.h>
    #include <string.h>
    #include <ctype.h>
     
    LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
     
    int rxPin = 0; // RX pin
    int txPin = 1; // TX pin
    int byteGPS=-1;
    char cmd[7] = "$GPRMC";
    int counter1 = 0; // counts how many bytes were received (max 300)
    int counter2 = 0; // counts how many commas were seen
    int offsets[13];
    char buf[300] = "";
     
    /**
    * Setup display and gps
    */
    void setup() {
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    Serial.begin(4800);
    lcd.begin(16, 2);
    lcd.print("waiting for gps");
    offsets[0] = 0;
    reset();
    }
     
    void reset() {
    counter1 = 0;
    counter2 = 0;
    }
     
    int get_size(int offset) {
    return offsets[offset+1] - offsets[offset] - 1;
    }
     
    int handle_byte(int byteGPS) {
    buf[counter1] = byteGPS;
    Serial.print((char)byteGPS);
    counter1++;
    if (counter1 == 300) {
    return 0;
    }
    if (byteGPS == ',') {
    counter2++;
    offsets[counter2] = counter1;
    if (counter2 == 13) {
    return 0;
    }
    }
    if (byteGPS == '*') {
    offsets[12] = counter1;
    }
     
    // Check if we got a <LF>, which indicates the end of line
    if (byteGPS == 10) {
    // Check that we got 12 pieces, and that the first piece is 6 characters
    if (counter2 != 12 || (get_size(0) != 6)) {
    return 0;
    }
     
    // Check that we received $GPRMC
    for (int j=0; j<6; j++) {
    if (buf[j] != cmd[j]) {
    return 0;
    }
    }
     
    // Check that time is well formed
    if (get_size(1) != 10) {
    return 0;
    }
     
    // Check that date is well formed
    if (get_size(9) != 6) {
    return 0;
    }
     
    // TODO: compute and validate checksum
     
    // TODO: handle timezone offset
     
    // print time
    lcd.clear();
    for (int j=0; j<6; j++) {
    lcd.print(buf[offsets[1]+j]);
    if (j==1) {
    lcd.print("h");
    } else if (j==3) {
    lcd.print("m");
    } else if (j==5) {
    lcd.print("s UTC");
    }
    }
     
    // print date
    lcd.setCursor(0, 1);
    for (int j=0; j<6; j++) {
    lcd.print(buf[offsets[9]+j]);
    if (j==1 || j==3) {
    lcd.print(".");
    }
    }
    return 0;
    }
    return 1;
    }
     
    /**
    * Main loop
    */
    void loop() {
    byteGPS=Serial.read(); // Read a byte of the serial port
    if (byteGPS == -1) { // See if the port is empty yet
    delay(100);
    } else {
    if (!handle_byte(byteGPS)) {
    reset();
    }
    }
    }
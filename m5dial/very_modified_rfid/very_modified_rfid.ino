#include "M5Dial.h"

void setup() {
    auto cfg = M5.config();
    M5Dial.begin(cfg, false, true);
    M5Dial.Display.setTextColor(GREEN);
    M5Dial.Display.setTextDatum(middle_center);
    M5Dial.Display.setTextFont(&fonts::Orbitron_Light_32);
    M5Dial.Display.setTextSize(1);

    M5Dial.Display.drawString("RFID Card", M5Dial.Display.width() / 2,
                              M5Dial.Display.height() / 2);
}

void loop() {
// Look for new cards, and select one if present
    if (!M5Dial.Rfid.PICC_IsNewCardPresent() || !M5Dial.Rfid.PICC_ReadCardSerial()) {
        delay(50);
        return;
    }

    // Now a card is selected. The UID and SAK is in M5Dial.Rfid.

    // Dump UID
    Serial.print(F("Card UID:"));
    M5.Lcd.println(" ");
    for (byte i = 0; i < M5Dial.Rfid.uid.size; i++) {
        Serial.print(M5Dial.Rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(M5Dial.Rfid.uid.uidByte[i], HEX);
        M5.Lcd.print(M5Dial.Rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        M5.Lcd.print(M5Dial.Rfid.uid.uidByte[i], HEX);
    }
    Serial.println();
}

void ShowReaderDetails() {
    // Get the MFRC522 software version
    byte v = M5Dial.Rfid.PCD_ReadRegister(M5Dial.Rfid.VersionReg);
    Serial.print(F("MFRC522 Software Version: 0x"));
    Serial.print(v, HEX);
    if (v == 0x91)
        Serial.print(F(" = v1.0"));
    else if (v == 0x92)
        Serial.print(F(" = v2.0"));
    else
        Serial.print(F(" (unknown)"));
    Serial.println("");
    // When 0x00 or 0xFF is returned, communication probably failed
    if ((v == 0x00) || (v == 0xFF)) {
        Serial.println(
            F("WARNING: Communication failure, is the MFRC522 properly "
              "connected?"));
    }
}

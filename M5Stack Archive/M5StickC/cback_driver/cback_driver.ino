#include "src/M5StickC/M5StickC.h"

#include "CBACK_DRIVER.h"

#define PAN 1
#define TILT 2

#define TILT_UP 180 //0 is flat down
#define PAN_CENTER 90 //0 is point to left, 180 is point to right

bool flag = true;

CBACK_DRIVER Driver;

void setup() {
    M5.begin();
    Driver.Init();
    Serial.println("C Back Driver Test...");
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setRotation(1);
    M5.Lcd.drawCentreString("C BACK Driver", 70, 15, 2);
    M5.Lcd.drawCentreString("Click Btn A Start", 70, 35, 2);

    Driver.SetServoAngle(PAN, PAN_CENTER);
    Driver.SetServoAngle(TILT, TILT_UP);
    Driver.SetOutput(1); //??
    delay(3000);
    Driver.SetOutput(0); //??
}

void loop() {

  /*
    if (M5.BtnA.wasPressed()) {
        flag = !flag;
    }
    if (flag) {
        for (int i = 1; i <= 4; i++) {
            Driver.SetServoAngle(i, 90);
            // Driver.ReadServoPulse(i,1500);
        }
        Driver.SetOutput(1);
    } else {
        for (int i = 1; i <= 4; i++) {
            Driver.SetServoAngle(i, 120);
            // Driver.ReadServoPulse(i,2000);
        }
        Driver.SetOutput(0);
    }
    Serial.print("PortB ADC: ");
    Serial.println(Driver.ReadADC());
    */
    M5.update();
}

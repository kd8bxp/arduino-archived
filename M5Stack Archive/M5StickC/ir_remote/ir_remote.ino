#include "src/M5StickC/M5StickC.h"

int ir_recv_pin = 33;  // set the input pin.  设置引脚
int ir_send_pin = 32;

int last_recv_value = 0;
int cur_recv_value  = 0;

void setup() {
    M5.begin();
    //M5.Power.begin();
    pinMode(ir_recv_pin, INPUT);
    pinMode(ir_send_pin, OUTPUT);
    // send infrared light.  发送红外线
    // now, you can see the infrared light through mobile phone camera.
    // 现在，你可以通过手机摄像头看到红外光
    digitalWrite(ir_send_pin, 1);
    M5.Lcd.setRotation(3);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Test for IR receiver: ");
    M5.Lcd.setTextSize(2);
}

void loop() {
    // now, once you press the button on a remote controller to send infrared
    // light.  现在，一旦你按下遥控器上的按钮发送红外线 the screen will display
    // "detected!"  屏幕将显示“检测到!”
    cur_recv_value = digitalRead(ir_recv_pin);
    if (last_recv_value != cur_recv_value) {
        M5.Lcd.setCursor(0, 25);
        M5.Lcd.fillRect(0, 25, 150, 25, BLACK);
        if (cur_recv_value ==
            0) {  // 0: detected 1: not detected,  0检测到,1没有检测到
            M5.Lcd.print("detected!");
        }
        last_recv_value = cur_recv_value;
    }
    Serial.println(cur_recv_value);
}

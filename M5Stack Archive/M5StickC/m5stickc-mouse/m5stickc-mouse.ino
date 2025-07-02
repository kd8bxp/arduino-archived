#include <cmath>

#include "src/M5StickC/M5StickC.h"

//#include <BLEDevice.h>
#include "src/BleMouse/BleMouse.h"

#include "joystick-position.h"
#include "joystick-hat.h"

namespace {
    std::string device_name("M5StickC Mouse");
    std::string device_manufacture("nnm_t");

    constexpr const int8_t divider = 8;
    constexpr const int8_t threshold = 3;

    BleMouse ble_mouse(device_name, device_manufacture, 100);
    JoystickHat joystick;
}

void setup()
{
    M5.begin();
    ble_mouse.begin();
    joystick.begin();

    M5.Lcd.setTextFont(2);
}

void loop()
{
    M5.update();

    if (ble_mouse.isConnected())
    {
        M5.Lcd.drawString("BLE Ready", 0, 0);
    }

    const JoystickPosition pos = joystick.read();

    M5.Lcd.fillRect(0, 32, 80, 16, TFT_BLACK);
    if (pos.getIsPressed())
    {
        ble_mouse.press(MOUSE_LEFT);
        M5.Lcd.drawString("LeftClick", 0, 32);
    }
    else
    {
        ble_mouse.release(MOUSE_LEFT);
    }

    // 相対値
    int8_t x = pos.getX() / divider;
    int8_t y = pos.getY() * -1 / divider;
    M5.Lcd.fillRect(0, 16, 80, 16, TFT_BLACK);
    M5.Lcd.drawString("X=" + String(x) + ", Y=" + String(y), 0, 16);
    if (std::abs(x) < threshold && std::abs(y) < threshold)
    {
        // しきい値以下は無視
    }
    else
    {
        ble_mouse.move(x, y, 0, 0);
    }

    delay(10);
}

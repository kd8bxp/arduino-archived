#include <M5EPD.h>
#include "epdgui.h"
#include "frame.h"
#include "systeminit.h"

void setup()
{
    pinMode(M5EPD_MAIN_PWR_PIN, OUTPUT);
    M5.enableMainPower();

    SysInit_Start();
}


void loop()
{
    EPDGUI_MainLoop();
}

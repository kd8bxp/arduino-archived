//The RCA Unit works with The Core unit that has the black connector on it
// it uses the I2S bus, and that is on the black connector
// it may work with other controllers that use PIN 26
// maybe a small adaptor could be made for the StickC


//compiles with M5Stack core 2.0.7

#include "src/M5Unified/M5Unified.h"
#include "src/M5GFX/M5UnitRCA.h"
#include "image.h"

M5UnitRCA gfx_rca;



void setup(void) {
    auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;

    M5.begin();

    M5.Display.clear(TFT_BLACK);
    M5.Display.setFont(&DejaVu24);
    
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.drawCenterString("M5 RCA Module :)", 160, 20);

    M5.Display.drawCenterString("Using Pin 26", 160, 120);
    M5.Display.drawString("Pin 25", 25, 210);
    M5.Display.drawString("Pin 26", 220, 210);

     if (M5.getBoard() == m5gfx::boards::board_M5StackCore2) {
         {
             auto spk_cfg        = M5.Speaker.config();
             spk_cfg.i2s_port    = I2S_NUM_1;
             spk_cfg.sample_rate = 96000;

             spk_cfg.pin_data_out = 2;
             spk_cfg.pin_bck      = 19;
             spk_cfg.pin_ws       = 0;  // LRCK

             spk_cfg.stereo        = true;
             spk_cfg.buzzer        = false;
             spk_cfg.use_dac       = false;
             spk_cfg.magnification = 16;
             M5.Speaker.config(spk_cfg);
         }
     } else {
         auto spk_cfg        = M5.Speaker.config();
         spk_cfg.i2s_port    = I2S_NUM_1;
         spk_cfg.sample_rate = 96000;

         spk_cfg.pin_data_out = 15;
         spk_cfg.pin_bck      = 13;
         spk_cfg.pin_ws       = 0;  // LRCK

         spk_cfg.stereo        = true;
         spk_cfg.buzzer        = false;
         spk_cfg.use_dac       = false;
         spk_cfg.magnification = 16;
         M5.Speaker.config(spk_cfg);
     }

     M5.Speaker.begin();
     M5.Speaker.setVolume(128);
     M5.Speaker.setChannelVolume(0, 255);
     M5.Speaker.setChannelVolume(1, 255);

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();
    
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
}


void loop(void) {

gfx_rca.drawJpg( downtoleft  // data_pointer
                 , ~0u  // data_length (~0u = auto)
                 , 0    // X position
                 , 0    // Y position
                 , gfx_rca.width()  // Width
                 , gfx_rca.height() // Height
                 , 0    // X offset
                 , 0    // Y offset
                 , 1.0  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
                 , 1.0  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
                 , datum_t::top_center
                 );
    delay(1000);

gfx_rca.drawJpg( downtoright  // data_pointer
                 , ~0u  // data_length (~0u = auto)
                 , 0    // X position
                 , 0    // Y position
                 , gfx_rca.width()  // Width
                 , gfx_rca.height() // Height
                 , 0    // X offset
                 , 0    // Y offset
                 , 1.0  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
                 , 1.0  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
                 , datum_t::top_center
                 );
    delay(1000);

gfx_rca.drawJpg( angry  // data_pointer
                 , ~0u  // data_length (~0u = auto)
                 , 0    // X position
                 , 0    // Y position
                 , gfx_rca.width()  // Width
                 , gfx_rca.height() // Height
                 , 0    // X offset
                 , 0    // Y offset
                 , 1.0  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
                 , 1.0  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
                 , datum_t::top_center
                 );
    delay(1000);
  
}

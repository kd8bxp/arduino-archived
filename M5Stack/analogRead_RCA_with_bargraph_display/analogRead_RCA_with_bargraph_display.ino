//The RCA Unit works with The Core unit that has the black connector on it
// it uses the I2S bus, and that is on the black connector
// it may work with other controllers that use PIN 26
// maybe a small adaptor could be made for the StickC

#include "src/M5Unified/M5Unified.h"
#include "src/M5GFX/M5UnitRCA.h"

#include "wav_unsigned_8bit_click.cpp"
#include "space_age8pt7b.h"


#define M5STACKFIRE_MICROPHONE_PIN 34
#define HORIZONTAL 216

int micValue = 0;
double capture[HORIZONTAL]; 
double oldCapture[HORIZONTAL];

M5UnitRCA gfx_rca;

static constexpr size_t BAR_COUNT = 64;
static int max_y[BAR_COUNT];
static int prev_y[BAR_COUNT];
static uint32_t colors[BAR_COUNT];


// M5UnitRCA gfx_rca ( 216                            // logical_width
//                   , 144                            // logical_height
//                   , 256                            // output_width
//                   , 160                            // output_height
//                   , M5UnitRCA::signal_type_t::PAL  // signal_type
//                   , 26                             // GPIO pin
//                   );
// signal_type: can be selected from NTSC / NTSC_J / PAL / PAL_M / PAL_N.
// GPIO pin: can be selected from 25 / 26

static constexpr int color_list[5]   = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_PURPLE, TFT_YELLOW};
static int color_index               = 0;
static unsigned long last_clear_time = 0;
static bool is_pin_25                = true;

/*void play_wav(void*) {
    while (true) {
        M5.Speaker.playWav(sunrise_anna_single_left,
                           sizeof(sunrise_anna_single_left), ~0u, 0, true);
        delay(3000);
        M5.Speaker.stop(0);
        delay(500);

        M5.Speaker.playWav(sunrise_anna_single_right,
                           sizeof(sunrise_anna_single_right), ~0u, 1, true);
        delay(3000);
        M5.Speaker.stop(1);
        delay(500);
    }
}
*/
void setup(void) {
    auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;

    M5.begin();

    M5.Display.clear(TFT_BLACK);
   /* M5.Display.setFont(&DejaVu24);
    //M5.Display.setFont(&space_age8pt7b);
    M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Display.drawCenterString("M5 RCA Module :)", 160, 20);

    M5.Display.drawCenterString("Using Pin 26", 160, 120);
    M5.Display.drawString("Pin 25", 25, 210);
    M5.Display.drawString("Pin 26", 220, 210);
*/
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
    //gfx_rca.setFont(&space_age8pt7b);
    // xTaskCreatePinnedToCore(play_wav, "play_wav", 8192, NULL, 1, NULL, 1);

  /*   for (int x = 0; x < BAR_COUNT; ++x)
  {
    prev_y[x] =  gfx_rca.height();
    max_y[x] =  gfx_rca.height();
    int r=0,g=0,b=0;
    switch (x >> 4)
    {
    case 0:
      b = 255;
      g = x*0x11;
      break;
    case 1:
      b = 255 - (x&15)*0x11;
      g = 255;
      break;
    case 2:
      g = 255;
      r = (x&15)*0x11;
      break;
    case 3:
      r = 255;
      g = 255 - (x&15)*0x11;
      break;
    }
    colors[x] =  gfx_rca.color888(r,g,b);
  }

    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    */
}

//int blink_flag = 0;
void loop(void) {
//int h =  gfx_rca.height();

 /* static int i;
  ++i;
   gfx_rca.waitDisplay();
  for (int x = 0; x < BAR_COUNT; ++x)
  {
    int y = (h>>1) - (sinf((float)((x-24)*i) / 3210.0f) + sinf((float)((x-40)*i) / 1234.0f)) * (h>>2);

    int xpos = x *  gfx_rca.width() / BAR_COUNT;
    int w = ((x+1) *  gfx_rca.width() / BAR_COUNT) - xpos - 1;
    if (max_y[x]+1 >= y) { max_y[x] = y-1; }
    else
    {
      if ((i & 3) ==0 )
      {
         gfx_rca.fillRect(xpos, max_y[x]-3, w, 1, TFT_BLACK);
        max_y[x]++;
      }
    }
     gfx_rca.fillRect(xpos, max_y[x]-3, w, 3, TFT_WHITE);
    if (prev_y[x] < y)
    {
       gfx_rca.fillRect(xpos, prev_y[x], w, y - prev_y[x], TFT_BLACK);
    }
    else
    {
       gfx_rca.fillRect(xpos, y, w, prev_y[x] - y, colors[x]);
    }
    prev_y[x] = y;
  }
   gfx_rca.display();
   */

for (int i=0; i<50 /*HORIZONTAL*/; i++) {
     int readMicValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);
     
      micValue = readMicValue; //temp;
      if (micValue > 1950 && micValue < 2250) { readMicValue = 1950; }
      //if (micValue < 1800 ) { readMicValue = 1950; }
      //if (micValue < 1700 ) {readMicValue = 1700; }
      //if (micValue > 2250 ) {readMicValue = 2250; }
      //int temp = map(readMicValue, 1700, 4000, 60, 240);
      int temp = map(readMicValue, 1700, 4000, 60, 125);
      capture[i] = abs(temp); //readMicValue;
      
   }

for (int i=1; i<50 /*HORIZONTAL-1*/; i++) {
  if (capture[i] > 72) { speak(capture[i]); }
  //else {gfx_rca.clear();}
}

}

void speak(int c) {

for (int i=25; i>0; i-- ) {
  gfx_rca.fillRect(95, 50 + i , 5, 2, TFT_WHITE);
  gfx_rca.fillRect(101, 55+ i , 5, 2, TFT_WHITE);
  gfx_rca.fillRect(89, 55+ i , 5, 2, TFT_WHITE);
  gfx_rca.fillRect(95, 50+25-i, 5, 2, TFT_WHITE);
  gfx_rca.fillRect(101, 45+20 - i, 5, 2, TFT_WHITE);
  gfx_rca.fillRect(89, 45+20 - i, 5, 2, TFT_WHITE);
  //delay(10);
  
}

for (int i=25; i>0; i-- ) {
  gfx_rca.fillRect(95, 50 +i , 5, 2, TFT_BLACK);
  gfx_rca.fillRect(101, 55+ i, 5, 2, TFT_BLACK);
  gfx_rca.fillRect(89, 55+ i, 5, 2, TFT_BLACK);
  gfx_rca.fillRect(95, 50+25-i, 5, 2, TFT_BLACK);
  gfx_rca.fillRect(101, 45+20 - i, 5, 2, TFT_BLACK);
  gfx_rca.fillRect(89, 45 +20- i, 5, 2, TFT_BLACK);
  //delay(10);
}






   
   /* M5.update();

    if ((millis() - last_clear_time) > 1000) {
        last_clear_time = millis();

        color_index++;
        if (color_index > 4) {
            color_index = 0;
        }

        gfx_rca.clear(color_list[color_index]);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("M5 RCA Module :)", 120, 50);

        if (is_pin_25) {
            gfx_rca.drawCenterString("Using Pin 25", 120, 110);
        } else {
            gfx_rca.drawCenterString("Using Pin 26", 120, 110);
        }

        blink_flag = !blink_flag;
        digitalWrite(16, blink_flag);
        digitalWrite(17, blink_flag);
    }

    if (M5.BtnA.isPressed()) {
        if (is_pin_25) {
            return;
        }
        is_pin_25 = true;
        M5.Display.drawCenterString("Using Pin 25", 160, 120);

        gfx_rca.setOutputPin(25);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 25", 120, 110);
    }

    if (M5.BtnC.isPressed()) {
        if (!is_pin_25) {
            return;
        }
        is_pin_25 = false;
        M5.Display.drawCenterString("Using Pin 26", 160, 120);

        gfx_rca.setOutputPin(26);
        gfx_rca.setTextColor(TFT_WHITE, color_list[color_index]);
        gfx_rca.drawCenterString("Using Pin 26", 120, 110);
    }
    */
}

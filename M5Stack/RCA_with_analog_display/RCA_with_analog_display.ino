//The RCA Unit works with The Core unit that has the black connector on it
// it uses the I2S bus, and that is on the black connector
// it may work with other controllers that use PIN 26
// maybe a small adaptor could be made for the StickC

#include "src/M5Unified/M5Unified.h"
#include "src/M5GFX/M5UnitRCA.h"

#include "wav_unsigned_8bit_click.cpp"
#include "space_age8pt7b.h"

M5UnitRCA gfx_rca;

static constexpr float deg_to_rad = 0.017453292519943295769236907684886;
static constexpr int TFT_GREY = 0x5AEB;
static constexpr int LOOP_PERIOD = 35; // Display updates every 35 ms

int value[6] = {0, 0, 0, 0, 0, 0};
int old_value[6] = { -1, -1, -1, -1, -1, -1};
int liner_count = 6;
int d = 0;

int liner_height;
int meter_height;
int needle_x;
int needle_y;
int needle_r;
int osx, osy; // Saved x & y coords
uint32_t updateTime = 0;       // time for next update

static constexpr int color_list[5]   = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_PURPLE, TFT_YELLOW};
static int color_index               = 0;
static unsigned long last_clear_time = 0;
static bool is_pin_25                = true;

void plotNeedle(int value)
{
  float sdeg = map(value, 0, 100, -135, -45); // Map value to angle
  // Calcualte tip of needle coords
  float sx = cosf(sdeg * deg_to_rad);
  float sy = sinf(sdeg * deg_to_rad);

  gfx_rca.setClipRect(0, 0, gfx_rca.width(), meter_height - 5);
  // Erase old needle image
  gfx_rca.drawLine(needle_x - 1, needle_y, osx - 1, osy, TFT_WHITE);
  gfx_rca.drawLine(needle_x + 1, needle_y, osx + 1, osy, TFT_WHITE);
  gfx_rca.drawLine(needle_x    , needle_y, osx    , osy, TFT_WHITE);

  // Re-plot text under needle
  gfx_rca.setTextColor(TFT_BLACK);
  if (gfx_rca.width() > 100)
  {
    gfx_rca.setFont(&fonts::Font4);
    gfx_rca.setTextDatum(textdatum_t::middle_center);
    gfx_rca.drawString("%RH", needle_x, needle_y>>1);
  }

  gfx_rca.setFont(&fonts::Font2);
  gfx_rca.setTextDatum(textdatum_t::bottom_left);
  gfx_rca.drawString("%RH", gfx_rca.width() - 36, meter_height - 8);

  gfx_rca.setTextColor(TFT_BLACK, TFT_WHITE);
  gfx_rca.setTextDatum(textdatum_t::bottom_right);
  gfx_rca.drawNumber(value, 36, meter_height - 8);


  // Store new needle end coords for next erase
  osx = roundf(sx * needle_r) + needle_x;
  osy = roundf(sy * needle_r) + needle_y;

  // Draw the needle in the new postion, magenta makes needle a bit bolder
  // draws 3 lines to thicken needle
  gfx_rca.drawLine(needle_x - 1, needle_y, osx - 1, osy, TFT_RED);
  gfx_rca.drawLine(needle_x + 1, needle_y, osx + 1, osy, TFT_RED);
  gfx_rca.drawLine(needle_x    , needle_y, osx    , osy, TFT_MAGENTA);

  gfx_rca.clearClipRect();
}

void analogMeter()
{
  gfx_rca.fillRect(0, 0, gfx_rca.width()   , meter_height  , TFT_WHITE);
  gfx_rca.drawRect(1, 1, gfx_rca.width()-2 , meter_height-2, TFT_BLACK);

  int r3 = needle_y * 13 / 15;
  int r2 = needle_y * 12 / 15;
  int r1 = needle_y * 11 / 15;
  needle_r = r1 - 3;
  gfx_rca.fillArc(needle_x, needle_y, r1, r3, 270, 292, TFT_GREEN);
  gfx_rca.fillArc(needle_x, needle_y, r1, r3, 292, 315, TFT_ORANGE);
  gfx_rca.fillArc(needle_x, needle_y, r1, r1, 225, 315, TFT_BLACK);

  gfx_rca.setTextColor(TFT_BLACK);
  gfx_rca.setFont(&fonts::Font2);
  gfx_rca.setTextDatum(textdatum_t::bottom_center);
  for (int i = 0; i <= 20; i++)
  {
    gfx_rca.fillArc(needle_x, needle_y, r1, (i % 5) ? r2 : r3, 225 + i * 4.5f, 225 + i * 4.5f, TFT_BLACK);
    if (0 == (i % 5) && gfx_rca.width() > 100)
    {
      gfx_rca.drawNumber(i * 5, needle_x - cosf((45+i*4.5) * deg_to_rad) * r3
                              , needle_y - sinf((45+i*4.5) * deg_to_rad) * r3 - 2);
    }
  }
}

void plotLinear(const char *label, int x, int y, int w, int h)
{
  gfx_rca.drawRect(x, y, w, h, TFT_GREY);
  gfx_rca.fillRect(x + 2, y + 18, w - 3, h - 36, TFT_WHITE);
  gfx_rca.setTextColor(TFT_CYAN, TFT_BLACK);
  gfx_rca.setTextDatum(textdatum_t::middle_center);
  gfx_rca.setFont(&fonts::Font2);
  gfx_rca.setTextPadding(gfx_rca.textWidth("100"));
  gfx_rca.drawString(label, x + w / 2, y + 10);

  int plot_height = h - (19*2);

  for (int i = 0; i <= 10; i ++)
  {
    gfx_rca.drawFastHLine(x + w/2, y + 19 + (i+1) * plot_height / 12, w*(3+(0==(i%5)))/16, TFT_BLACK);
  }
}

void plotPointer(void)
{
  gfx_rca.setTextColor(TFT_GREEN, TFT_BLACK);
  gfx_rca.setTextDatum(textdatum_t::middle_right);

  int plot_height = liner_height - (19*2);
  int pw = (gfx_rca.width() / liner_count) / 3;

  for (int i = 0; i < liner_count; i++)
  {
    gfx_rca.drawNumber(value[i], gfx_rca.width() * (i + 0.8) / liner_count, gfx_rca.height() - 10);
    int dx = gfx_rca.width() * i / liner_count + 3;
    int dy = meter_height + 19 + (old_value[i]+10) * plot_height / 120;
    gfx_rca.fillTriangle(dx, dy - 5, dx, dy + 5, dx + pw, dy, TFT_WHITE);
    old_value[i] = value[i];
    dy = meter_height + 19 + (old_value[i]+10) * plot_height / 120;
    gfx_rca.fillTriangle(dx, dy - 5, dx, dy + 5, dx + pw, dy, TFT_RED);
  }
}

void setup(void) {
    auto cfg         = M5.config();
    cfg.external_spk = true;
    cfg.internal_spk = false;

    M5.begin();

    M5.Display.clear(TFT_BLACK);
    M5.Display.setFont(&DejaVu24);
    //M5.Display.setFont(&space_age8pt7b);
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

 

    gfx_rca.init();
    gfx_rca.setFont(&DejaVu24);
    gfx_rca.startWrite();

   liner_height = gfx_rca.height() * 3 / 5;
  meter_height = gfx_rca.height() * 2 / 5;
  needle_x = gfx_rca.width() / 2;
  needle_y = (meter_height*2 + gfx_rca.width()) / 3;
  osx = needle_x;
  osy = needle_y;
  liner_count = std::min(6, gfx_rca.width() / 40);

  analogMeter(); // Draw analogue meter
  int w = gfx_rca.width() / liner_count;
  char name[] = "A0";
  // Draw linear meters
  for (int i = 0; i < liner_count; i++)
  {
    name[1] = '0' + i;
    plotLinear(name, gfx_rca.width() * i / liner_count, meter_height, w, liner_height);
  }
  gfx_rca.endWrite();

  updateTime = millis(); // Next update time



    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
}

int blink_flag = 0;
void loop(void) {
 auto msec = millis();
  if (updateTime <= msec)
  {
    d = (msec >> 3) % 360;

    // Create a Sine wave for testing
    value[0] = 50 + roundf(50 * sinf((d +   0) * deg_to_rad));
    value[1] = 50 + roundf(50 * sinf((d +  60) * deg_to_rad));
    value[2] = 50 + roundf(50 * sinf((d + 120) * deg_to_rad));
    value[3] = 50 + roundf(50 * sinf((d + 180) * deg_to_rad));
    value[4] = 50 + roundf(50 * sinf((d + 240) * deg_to_rad));
    value[5] = 50 + roundf(50 * sinf((d + 300) * deg_to_rad));

    //value[0] = map(analogRead(36), 0, 4095, 0, 100); // Test with value form GPIO36

    if (!gfx_rca.displayBusy())
    {
      updateTime = msec + LOOP_PERIOD;
    //unsigned long t = millis();
      gfx_rca.startWrite();
      plotPointer();
      plotNeedle(roundf(value[0]));
      gfx_rca.endWrite();
    //Serial.println(millis()-t); // Print time taken for meter update
    }
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

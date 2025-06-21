/* The Orville Font Display Test
 *  The font is the called Space Age Font and can be downloaded from
 *  https://fontmeme.com/fonts/space-age-font/
 *  
 *  The rendering of the ship was found here:
 *  https://blenderartists.org/t/the-orville/1117094 by Belistner
 *  resized, background removed (converted to alpha), using gimp and converted to grayscale
 *  by the provided image convert script
 *  
 *  LeRoy Miller (c) 2021
 */
 
#include <Arduino.h>
#include "src/epd_driver.h"
#include "spaceagerotate16.h"
#include "spaceage20.h"
#include "TheOrville.h"

uint8_t *framebuffer;

void setup() {
    Serial.begin(115200);
   
    epd_init();
//framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
//    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);

framebuffer = (uint8_t *)heap_caps_malloc(EPD_HEIGHT * EPD_WIDTH / 2, MALLOC_CAP_SPIRAM);
    memset(framebuffer, 0xFF, EPD_HEIGHT * EPD_WIDTH / 2);

}

void loop() {
  delay(300);

    epd_poweron();
    volatile uint32_t t1 = millis();
    epd_clear();
    volatile uint32_t t2 = millis();
    printf("EPD clear took %dms.\n", t2 - t1);
    epd_poweroff();

    epd_draw_hline(20, 20, EPD_WIDTH - 40, 0x00, framebuffer);
    epd_draw_hline(20, EPD_HEIGHT - 20, EPD_WIDTH - 40, 0x00, framebuffer);
    epd_draw_vline(20, 20, EPD_HEIGHT - 40 + 1, 0x00, framebuffer);
    epd_draw_vline(EPD_WIDTH - 20, 20, EPD_HEIGHT - 40 + 1, 0x00, framebuffer);

    Rect_t area = {
        .x = 20, //230,
        .y = 230, //20,
        .width = TheOrville_width,
        .height = TheOrville_height,
    };
    epd_poweron();
    //epd_draw_grayscale_image(area, (uint8_t *)TheOrville_data);
    epd_copy_to_framebuffer(area, (uint8_t *)TheOrville_data, framebuffer);
    //epd_draw_grayscale_image(epd_full_screen(), framebuffer);
    epd_poweroff();
    int cursor_x = 220;
    int cursor_y = 350;
    char *string1 = "The Orville";
    char *string2 = "ECV-197";
   
    
epd_poweron();
    writeln((GFXfont *)&spaceage20, string1, &cursor_y, &cursor_x, framebuffer); //NULL);
   epd_draw_grayscale_image(epd_full_screen(), framebuffer);
    delay(500);
    cursor_x = 270;
    cursor_y += 50;
    writeln((GFXfont *)&spaceage20, string2, &cursor_y, &cursor_x, NULL);
    delay(500);
    

    epd_poweroff();

   //rotated();
    delay(5000);

}

void rotated() {
  epd_poweron();
   int cursor_x = 0;
    int cursor_y = 540;
    writeln((GFXfont *)&spaceagerotate16, "T", &cursor_x, &cursor_y, NULL);
    delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "h", &cursor_x, &cursor_y, NULL);
    delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "e", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, " ", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "O", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "r", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "v", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "i", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "l", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "l", &cursor_x, &cursor_y, NULL);
     delay(500);
     cursor_x = 0;
     cursor_y -= 30;
    writeln((GFXfont *)&spaceagerotate16, "e", &cursor_x, &cursor_y, NULL);
    delay(500);
     epd_poweroff();
}

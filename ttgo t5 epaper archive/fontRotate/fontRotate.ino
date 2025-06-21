/*  fontRotate using opensans regular 16 pt (opensans is under the Apache License see data directory)
 *   
 *   concept, work in progress, and not quite right.
 *   Rotated the opensans font using Fontforge, and converting to hex.
 *   This uses the framebuffer to write the rotated font.
 *   problems, spacing is off (in some cases quite a bit off, not sure how to fix that), the 2nd rotated line displays extra information (a row of 2s)
 *   not sure what is going on there, or what is going on.
 *   
 *   It's my hopes that someone that knows what they are doing can fix this proof of concept.
 *   All I ask is that I get at least an honorable mention for at least the idea. :-) 
 *   
 *  LeRoy Miller (c) 2021
 */
 
#include <Arduino.h>
#include "src/epd_driver.h"
#include "opensans-regular.h"
#include "opensans-regular90.h"

uint8_t *framebuffer;

void setup() {
    Serial.begin(115200);
   
    epd_init();
framebuffer = (uint8_t *)heap_caps_malloc(EPD_WIDTH * EPD_HEIGHT / 2, MALLOC_CAP_SPIRAM);
    memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);


}

void loop() {
  delay(300);

    epd_poweron();
    volatile uint32_t t1 = millis();
    epd_clear();
    volatile uint32_t t2 = millis();
    printf("EPD clear took %dms.\n", t2 - t1);
    epd_poweroff();

  
    int cursor_x = 220;
    int cursor_y = 350;
    char *string1 = "OpenSans Regular"; // 16pt rotated 90 degrees";
    char *string2 = "OpenSans Regular 16pt";
    char *string3 = "16pt rotated";
    
    loadBuffer(string1, 20, 540); //remember the screen is still techonoly in landscape.
    //loadBuffer(string3, 50, 540); //adding this line caused problems (?????)
    
    epd_poweron();
    writeln((GFXfont *)&OpenSansRegular, string2, &cursor_y, &cursor_x, framebuffer); //landscape display of text
    epd_draw_grayscale_image(epd_full_screen(), framebuffer); //update the display, with contents of framebuffer
    delay(500);
    epd_poweroff();

    delay(5000); //wait 5 seconds to display next string
    
memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2); //this clears the framebuffer
 loadBuffer(string3, 50, 540); //clearing the framebuffer still didn't fix this line (?????)
 epd_poweron();
 epd_clear();
 epd_draw_grayscale_image(epd_full_screen(), framebuffer);
    delay(500);
    epd_poweroff();
       
 while(1);
}

void loadBuffer(char temp[], int tempcursor_x, int tempcursor_y) {
  
    int cursor_x = tempcursor_x;  
    int cursor_y = tempcursor_y;
    char temp1;
    
    for (int i = 0; i <strlen(temp); i++) {
      temp1 = temp[i];
      if ((String)temp1 == " ") {cursor_y += 15;}
      
    writeln((GFXfont *)&OpenSansRegular90, &temp1, &cursor_x, &cursor_y, framebuffer);
    
     cursor_x = tempcursor_x;    
     cursor_y -= 30;
    }
    
}

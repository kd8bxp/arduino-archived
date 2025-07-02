/* This is for the Seeedstudio Grove Triple Color E-Ink Display 2.13"
 *  It mostly works, but seems like it has to be reset many times. 
 *  
 *  grove.h contains the seeedstudio logo, 
 *  batman.h displays a batman logo, but it's rotated in correctly and
 *  it's small. (the display also has a lot of extra garbage displayed)
 *  
 *  https://wiki.seeedstudio.com/Grove-Triple_Color_E-Ink_Display_2_13/
*/

// seems to work (ish) with M5 stack board core 2.0.7 
// have to hard reset the controller (pull power) This doesn't work well

#include "src/M5Atom/M5Atom.h"

#include "xbm.h" //works kind of
//#include "grove.h"
//#include "batman.h"

#define RECV_ERROR       -1
#define CONTINUE_TRANS    0
#define RECV_DONE         1


//Send data to e-link board.
void serial_send_data(const uint8_t* data, uint32_t data_len) {
    for (int i = 0; i < data_len; i++) {
        Serial1.write(pgm_read_byte(&data[i]));
    }
}

//Send image array
void write_image_picture(void) {
    for (int i = 0; i < 13; i++) {
        serial_send_data(&IMAGE_BLACK[0 + i * 212], 212);
        delay(80);
    }
    delay(90);
   // for (int i = 0; i < 13; i++) {
   //     serial_send_data(&IMAGE_RED[0 + i * 212], 212);
   //     delay(80);
   // }
}

//Send the start transfer command
void send_begin() {
    while (1) {
        if (Serial1.available() > 0) {
            char str0 = Serial1.read();
            if (str0 == 'c') {
                break;
            }
        }
    }
    char str = 'a';
    Serial1.write(str);
    while (1) {
        if (Serial1.available() > 0) {
            char str1 = Serial1.read();
            if (str1 == 'b') {
                break;
            }
        }
    }
}


void setup() {
    M5.begin();
    Serial1.begin(230400,SERIAL_8N1,32, 26); //26,32);
    delay(10);
    send_begin();
    delay(2000);
    write_image_picture();
}



void loop() {

}

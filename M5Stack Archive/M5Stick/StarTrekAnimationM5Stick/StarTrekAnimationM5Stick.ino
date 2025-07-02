/*
 *  Writen by Brian Heckathorne neosin@gmail.com Aug 2018 Godfear17 on github
 *  https://github.com/Godfear17/Oled-StarTrek-Animation
 */

#include "src/U8g2/U8g2lib.h"

// Change to your u8g2 constructor this is for the SH1106 I2C OLED
//U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SH1107_64X128_F_4W_HW_SPI u8g2(U8G2_R3, /* cs=*/ 14, /* dc=*/ 27, /* reset=*/ 33);

// Setup global variables 
int y; // Changes location height of 1st star on screen
int l; // changes the lenght of the 1st star on screen
int s; // changes the speed the stars go by 
int y2; // 2nd star height
int l2; //2nd star speed (will be subtracted by -10 to 10)
int em; // how much to move the whole ship

void setup() {

 u8g2.begin();

}


void loop() {
    // loops draw
    draw(); 
}

void draw(){

        em = random(-10,10); // how much to move the whole starship (all parts)
          
        y = random(0,64); // changes the height location of stars
        l = random(1,32); // changes the lenght of the stars
        s = random(16,64); // Changes the speed the stars go by

        y2 = random(0,64); // second star height
        l2 = random(1,32); // send star lenght
      
        // animation loop
        for (int a=0;a<128+l;a = a + s) {
        
        u8g2.clearBuffer(); // clear old buffer
        u8g2.drawHLine(a-l, y, l); // a-l so it flys into scene rather than appearing with out the -l

        u8g2.drawHLine(a-l2,y2,l2 - random(-10,10)); // 2nd star with some changes
        
        // top warp engine
        u8g2.drawCircle(72+em, 22, 3, U8G2_DRAW_UPPER_LEFT);
        u8g2.drawCircle(72+em, 22, 3, U8G2_DRAW_LOWER_LEFT);
        u8g2.drawLine(73+em, 19, 93+em, 19);
        u8g2.drawLine(73+em, 25, 93+em, 25);
        u8g2.drawCircle(93+em, 22, 2, U8G2_DRAW_UPPER_RIGHT);
        u8g2.drawCircle(93+em, 22, 2, U8G2_DRAW_LOWER_RIGHT);

        //bottom warp engine
        u8g2.drawCircle(72+em, 48, 3, U8G2_DRAW_UPPER_LEFT);
        u8g2.drawCircle(72+em, 48, 3, U8G2_DRAW_LOWER_LEFT);
        u8g2.drawLine(73+em, 45, 93+em, 45);
        u8g2.drawLine(73+em, 51, 93+em, 51);
        u8g2.drawCircle(93+em, 48, 2, U8G2_DRAW_UPPER_RIGHT);
        u8g2.drawCircle(93+em, 48, 2, U8G2_DRAW_LOWER_RIGHT);

        // midship body
        u8g2.drawEllipse(50+em, 35, 15, 20, U8G2_DRAW_ALL); //saucer section
        u8g2.drawEllipse(65+em, 30, 15, 4, U8G2_DRAW_UPPER_RIGHT);
        u8g2.drawEllipse(65+em, 40, 15, 4, U8G2_DRAW_LOWER_RIGHT);
        u8g2.drawCircle(81+em, 26, 4, U8G2_DRAW_LOWER_RIGHT);
        u8g2.drawCircle(81+em, 44, 4, U8G2_DRAW_UPPER_RIGHT);
        u8g2.drawEllipse(88+em, 35, 2, 10, U8G2_DRAW_UPPER_RIGHT);
        u8g2.drawEllipse(88+em, 35, 2, 10, U8G2_DRAW_LOWER_RIGHT);

        // decorative center lines
        u8g2.drawLine(65+em, 35, 80+em, 35);
        u8g2.drawLine(65+em, 30, 75+em, 35);u8g2.drawLine(65+em, 40, 75+em, 35);
        
        u8g2.sendBuffer(); //write the buffer to screen
        
        delay(100); // If you'd like to slow it down
        
        }
}

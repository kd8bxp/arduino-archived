#include "FastLED.h"
#include "DisplayCurrentTime.h"

//below needs to be fixed for the 126 matrix display
// this might help
//https://macetech.github.io/FastLED-XY-Map-Generator/
//no serpentine, no vertical, no v or h flip, width 18, height 7

int zero[16]  = {1,2,20,37,54,71,88,105,104,86,69,52,35,18};                          // Lattice of Number 0
int one[10]   = {19,2,20,38,56,74,92,110,109,111};                                                  // Lattice of Number 1
int two[12]   = {18,1,2,21,39,56,73,90,108,109,110,111};                                  // Lattice of Number 2
int three[11] = {18,1,2,21,39,56,75,93,110,109,90};                                 // Lattice of Number 3
int four[13]  = {1,3,19,21,37,39,55,56,57,58,75,93,111};                                            // Lattice of Number 4
int five[15]  = {1,2,3,4,18,36,55,56,57,76,93,111,110,109,90};                                 // Lattice of Number 5
int six[18]   = {1,2,3,4,25,49,73,74,75,76,97,100,121,124,145,146,147,148};                          // Lattice of Number 6 need
int seven[10] = {1,2,3,4,28,51,75,99,123,147};                                                       // Lattice of Number 7 need
int eight[20] = {1,2,3,4,25,28,49,52,73,74,75,76,97,100,121,124,145,146,147,148};                    // Lattice of Number 8 need
int nine[18]  = {1,2,3,4,25,28,49,52,73,74,75,76,100,124,145,146,147,148};                           // Lattice of Number 9 need
int point[2]  = {36,72};// Lattice of character :

      
int lenth[11] = {16,10,12,11,13,15,18,10,20,18,2};
int *num[10]={zero,one,two,three,four,five,six,seven,eight,nine};
int index_led;
void displayCurrentTime(uint16_t time4Day)
{
    int offset = 0;
    int number  = 10000;
    while(number)
    {
        int n = time4Day / number;
        if(n!=0)
        {
            time4Day = time4Day % number;
        }
        
        for(index_led=0;index_led<lenth[n];index_led++)
        {
            int a=*(*(num+n)+index_led) + offset;
            
            leds[a] = CRGB::White;
        }
        
        if(offset==6)
            offset += 7;
        else
            offset += 6;
        
        number = number / 10;
        if(number==100)
            number = number / 10;
    }
}

void diaplayPoint()
{
    for(index_led=0;index_led<2;index_led++)
    {
        int b = point[index_led];
        leds[b] = CRGB::White;
    }
}

void clearPoint()
{
    for(index_led=0;index_led<2;index_led++)
    {
        int c = point[index_led];
        leds[c] = CRGB::Black;
    }
}

void clearLeds()
{
    for(index_led=0;index_led<126;index_led++)
    {
        leds[index_led] = CRGB::Black;
    } 
}




//void set_leds()  // input activation
//{   
//  led_clear(); 
   
//  leds[21].  /*CRGB::Red;*/ setRGB(result1.getAverage(),0,0); /*pixels.setPixelColor(2, pixels.Color(cells_in[0]*3, 0,0));*/ FastLED.show();//delay(2); 
//  leds[23].  /*CRGB::Red;*/ setRGB(result2.getAverage(),0,0); /*pixels.setPixelColor(5, pixels.Color(cells_in[1]*3, 0,0));*/ FastLED.show();//delay(2); 
//  leds[out].  /*CRGB::Blue;*/ setRGB(0,0,150); /*pixels.setPixelColor(out+18,150, 0, 0);*/ FastLED.show(); //delay(2); 
// FastLED.show();
//}

//void leds_out()  // output activation
//{ 
//  led_clear();  leds[out] = CHSV(0,0,150); /*pixels.setPixelColor(out+17, pixels.Color(0,0,150));*/ FastLED.show(); //delay(2);
//}

//void leds_training()  // show complete activation 
//{
//  led_clear();
//  leds[21] = CHSV((cells_in[0]*15),(cells_in[0]*18),(cells_in[0]*15)); /*pixels.setPixelColor(2, pixels.Color(cells_in[0]*3, 0,0));*/ FastLED.show(); //delay(2); 
//  leds[23] = CHSV((cells_in[1]*15),(cells_in[1]*18),(cells_in[1]*15)); /*pixels.setPixelColor(5, pixels.Color(cells_in[1]*3, 0,0));*/ FastLED.show(); //delay(2);
//  leds[10] = CHSV(winrate*10,winrate*20,winrate); FastLED.show();
//  leds[11] = CHSV(Err_sum*10,Err_sum*20,Err_sum); FastLED.show();
//  for(int i = cell_raster_hidden; i > 0 ; i--)
//  {
//    leds[i-1] = CHSV(0,map(cells_hidden[i]*2,0.9,100,0,180),0); /*pixels.setPixelColor(i+8,  pixels.Color(0,map(cells_hidden[i]*2,0.9,100,0,180),0));*/
//    FastLED.show();
//  }
//  for(int i = cell_raster_out; i > 0; i--)
//  {
//   leds[i-1] = CHSV(0,0,map(cells_out[i]*2,0.6,100,0,180)); /* pixels.setPixelColor(i+17, pixels.Color(0,0,map(cells_out[i]*2,0.6,100,0,180))); */
//   FastLED.show();
//  }   
//  //FastLED.show(); //delay(2);
//}


//void led_clear() {
//  for (int iii=0; iii<NUM_LEDS; iii++) {
//    leds[iii].setRGB(0,0,0);
//      }
//    FastLED.show();
//}

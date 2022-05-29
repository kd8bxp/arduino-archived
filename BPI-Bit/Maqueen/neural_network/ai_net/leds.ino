void leds()  // input activation
{   
  pixels.clear();  
  pixels.setPixelColor(2, pixels.Color(cells_in[0]*3, 0,0)); pixels.show();delay(2); 
  pixels.setPixelColor(5, pixels.Color(cells_in[1]*3, 0,0)); pixels.show();delay(2); 
  pixels.setPixelColor(out+18,150, 0, 0); pixels.show(); delay(2); 
 //pixels.show();
}

void leds_out()  // output activation
{ 
  pixels.clear();  pixels.setPixelColor(out+17, pixels.Color(0,0,150)); pixels.show();delay(2);
}

void leds_training()  // show complete activation 
{
  pixels.clear();
  pixels.setPixelColor(2, pixels.Color(cells_in[0]*3, 0,0)); pixels.show();delay(2); 
  pixels.setPixelColor(5, pixels.Color(cells_in[1]*3, 0,0)); pixels.show();delay(2);
  for(int i = cell_raster_hidden; i > 0 ; i--)
  {
    pixels.setPixelColor(i+8,  pixels.Color(0,map(cells_hidden[i]*2,0.9,100,0,180),0));
  }
  for(int i = cell_raster_out; i > 0; i--)
  {
    pixels.setPixelColor(i+17, pixels.Color(0,0,map(cells_out[i]*2,0.6,100,0,180))); 
  }   
  pixels.show(); delay(2);
}

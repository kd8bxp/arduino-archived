void outvalue()  // get the output with the highest value
{
  double o = cells_out[0]; 
  out = 0; 
  for(int i = 0; i < cell_raster_out; i++)
  {    
    if(cells_out[i] >= o)
    {
      o = cells_out[i];
      out = i; 
    }  
  } 
}

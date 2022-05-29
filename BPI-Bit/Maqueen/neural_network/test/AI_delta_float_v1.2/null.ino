void rst()  
{
  for(int c = 0; c < cell_raster_in; c++)
  {  
    cells_in[c] = 0;  
  }
  for(int c = 0; c < cell_raster_hidden; c++)
  {   
    cells_hidden[c] = 0;    
  } 
  for(int c = 0; c < cell_raster_out; c++)
  {  
    cells_out[c] = 0;
  }   
}

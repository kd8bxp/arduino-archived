void learning()  // train the network
{  
  double sum = 0;

  for(int x = 0 ; x < cell_raster_hidden ; x++ ) 
  {    
    Accum = 0.0 ;
    for(int y = 0 ; y < cell_raster_out ; y++ )
    {
      Accum += cells_hidden_out_weight[x][y] * Delta_out[y] ;
    }
  Delta_hidden[x] = Accum * cells_hidden[x] * (1.0 - cells_hidden[x]) ;
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {    
    cells_in_hidden_weight_change[cell_raster_in][x] =  epsilon * Delta_hidden[x] + mom * cells_in_hidden_weight_change[cell_raster_in][x] ;
    cells_in_hidden_weight[cell_raster_in][x] += cells_in_hidden_weight_change[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {       
      cells_in_hidden_weight_change[y][x] =  epsilon * test_in[p][y] * Delta_hidden[x] + mom * cells_in_hidden_weight_change[y][x] ;
      cells_in_hidden_weight[y][x] +=  cells_in_hidden_weight_change[y][x];    
    }
  }
  
  for(int x = 0; x < cell_raster_out; x++)
  {
    cells_hidden_out_weight_change[cell_raster_hidden][x] = epsilon *  Delta_out[x] + mom * cells_hidden_out_weight_change[cell_raster_hidden][x] ;
    cells_hidden_out_weight[cell_raster_hidden][x] += cells_hidden_out_weight_change[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {     
      cells_hidden_out_weight_change[y][x] =  epsilon * cells_hidden[y] * Delta_out[x]  + mom * cells_hidden_out_weight_change[y][x] ;
      cells_hidden_out_weight[y][x] += cells_hidden_out_weight_change[y][x];
    }
  } 
}

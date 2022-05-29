void seed() // random seed the weights
{  
  double upper = 1000;
  double lower = 50;
  for(int x = 0; x < cell_raster_in; x++)
  {
  for (int sy = 0 ; sy < cell_raster_hidden; sy++)
    { randomSeed(analogRead(3)); //srand(time(0));       
      double tmp = double(random(100))/100;
      cells_in_hidden_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_in_hidden_weight_change [x][sy] = 0.0;
    }
  }
  
  for(int x = 0; x < cell_raster_hidden; x++)
  {
  for (int sy = 0 ; sy < cell_raster_out; sy++)
    { randomSeed(analogRead(A3));        
      double tmp = double(random(100))/100;
      cells_hidden_out_weight[x][sy] = 2.0 * (tmp - 0.5 ) * InitWeights;     
      cells_hidden_out_weight_change[x][sy] = 0.0;
    }
  }       
}

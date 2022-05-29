void load()  // load data through the network
{
  //  biasrandom();  // test with a random bias
  double sum = 1.0; 
  int c = 0;
  Error = 0.0;
  for(int x = 0; x < cell_raster_hidden; x++)  // input        
  {  
    sum = cells_in_hidden_weight[cell_raster_in][x];
    for(int y = 0; y < cell_raster_in; y++)
    {
      sum += cells_in[y] * cells_in_hidden_weight[y][x];         
    }   
    cells_hidden[x] = 1.0/(1.0 + exp(-sum));   //?
  }  
  for(int x = 0; x < cell_raster_out; x++)  // hidden  
  {
    sum = cells_hidden_out_weight[cell_raster_hidden][x];
    for(int y = 0; y < cell_raster_hidden; y++)
    {
      sum += cells_hidden[y] * cells_hidden_out_weight[y][x];          
    }   
    cells_out[x] =  1.0/(1.0 + exp(-sum));
    Delta_out[x] =  (test_out[p][x] - cells_out[x]) * cells_out[x] * (1.0 -cells_out[x]);  
    Error += 0.5 * (test_out[p][x] - cells_out[x]) * (test_out[p][x] - cells_out[x]);
  }
}

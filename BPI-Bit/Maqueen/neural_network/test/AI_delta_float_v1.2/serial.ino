//  some serial prints
void line()
{
   Serial.print("----------------------------------------------------------------------------------err-"); Serial.print(Err_sum, 5); Serial.print(" -- winrate "); Serial.println(winrate, 5); 
}

void serial_print_in()
{
  Serial.print("in_wert -->  ");   
  for(int i = 0; i < cell_raster_in; i++)
  {          
    Serial.print(i);  Serial.print(" = "); Serial.print(cells_in[i],5); Serial.print(" - ");
  }
  Serial.println(); Serial.println(); 
}

void serial_print_hidden()
{
  Serial.print("hidden_wert --> "); 
  for(int i = 0; i < cell_raster_hidden +1; i++)
  {
    Serial.print(i); Serial.print(" = "); Serial.print(cells_hidden[i],5); Serial.print(" - ");
  }
  Serial.println(); Serial.println();  
} 

void serial_print_out()
{ 
  Serial.print("  output_wert -->  ");      
  for(int i = 0; i < cell_raster_out; i++)
  {
    Serial.print(i); Serial.print(" = "); Serial.print(cells_out[i],5); Serial.print("    -  ");
  }
  Serial.print(" out -> "); outvalue(); Serial.print(out);Serial.println();  
}

void serial_print_out_soll()
{ 
  Serial.print("  output soll -->  ");      
  for(int i = 0; i < cell_raster_out; i++)
  {    
    Serial.print(i); Serial.print(" = "); Serial.print(test_out[p][i]); Serial.print("       -  ");      
  }
  Serial.print(p+1); 
  Serial.println();  Serial.println(); 
}


void serial_print_hidden_weights()
{
   Serial.println("hidden_weights -->  ");   
   for(int i = 0; i < cell_raster_in; i++)
   { 
      for(int j = 0; j < cell_raster_hidden; j++)
      {
         Serial.print(i); Serial.print(":");     Serial.print(j);  Serial.print(" = "); Serial.print(cells_in_hidden_weight[i][j],5);  Serial.print(" - ");
      }
      Serial.println();
   }
Serial.println();   Serial.println();
}


void serial_print_out_weights()
{
   Serial.println("output_weights -->  ");   
   for(int i = 0; i < cell_raster_out; i++)
   { 
      for(int j = 0; j < cell_raster_hidden; j++)
      {
          Serial.print(i); Serial.print(":");     Serial.print(j);  Serial.print(" = "); Serial.print(cells_hidden_out_weight[j][i],5); Serial.print(" - ");
      }
     Serial.println();
   }
Serial.println();   Serial.println();
}

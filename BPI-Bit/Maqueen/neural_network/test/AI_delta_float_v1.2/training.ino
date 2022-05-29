void training()
{  
 count++;
  for(p = 0; p < patern; p++)
  {    
    cells_in[input_A_toCell] = test_in[p][0]/1000;
    cells_in[input_B_toCell] = test_in[p][1]/1000;
   // serial_print_in();  
    load(); 
    learning();  
    if(count == count_to)
    {   
      serial_print_out();
      serial_print_out_soll();    
   // serial_print_hidden_weights(); 
   // serial_print_out_weights();  
      leds_training();     
    }
    if(count == patern+count_to) { count = 0; }   
    Err_sum += Error;    
  }
 if(count == 0)         { winrate = winrate - Err_sum; line(); winrate = Err_sum; }
 
 if(Err_sum <= success) { modus = 1; }
  Err_sum = 0.0; 
   
} 

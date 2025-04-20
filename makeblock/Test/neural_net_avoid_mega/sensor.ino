void sensor()   // make a measurement
{  
  remeasure:  
   
  double result1 = ultrL.distanceCm(); 
  double result2 = ultrR.distanceCm();
  if(result1 < 250 || result2 < 250)
  {
    cells_in[input_A_toCell] = result1/1000;  Serial.print(" A= "); Serial.print(cells_in[input_A_toCell],5);
    cells_in[input_B_toCell] = result2/1000;  Serial.print(" B= "); Serial.println(cells_in[input_B_toCell],5);
  }
  else
  {
    goto remeasure;   
  }   
}

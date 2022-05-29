void sensor()   // make a measurement
{  
  remeasure:  
  digitalWrite(sensor_A_out, LOW); delayMicroseconds(2); digitalWrite(sensor_A_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_A_out, LOW); int duration1 = pulseIn(sensor_A_in, HIGH);  
  digitalWrite(sensor_B_out, LOW); delayMicroseconds(2); digitalWrite(sensor_B_out, HIGH);  delayMicroseconds(10); digitalWrite(sensor_B_out, LOW); int duration2 = pulseIn(sensor_B_in, HIGH);
  // convert ToF to distance in cm
  double distance1 = duration1/58.3; 
  double distance2 = duration2/58.3;  
  double result1 = distance1;
  double result2 = distance2;
  if(distance1 < 250 || distance2 < 250)
  {
    cells_in[input_A_toCell] = result1/1000;  Serial.print(" A= "); Serial.print(cells_in[input_A_toCell],5);
    cells_in[input_B_toCell] = result2/1000;  Serial.print(" B= "); Serial.println(cells_in[input_B_toCell],5);
  }
  else
  {
    goto remeasure;   
  }   
}

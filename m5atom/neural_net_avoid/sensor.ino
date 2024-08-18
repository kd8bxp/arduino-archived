void readSensor()   // make a measurement
{  
  remeasure:  
  result1.clear();
  result2.clear();
  for (int ra=0; ra<10; ra++) {
  readRight();
  result1.addValue(dist);
  readLeft();
  result2.addValue(dist);
  }
    
  if(result1.getAverage() < 2000 || result2.getAverage() < 2000) //May 30 2024 back to 2000 //(use to be 2000) May 27 2024
  {
    double distance1 = result1.getAverage(); //abs(map(result1.getAverage(), 20, 1900, 0, 250)); // /10;
    double distance2 = result2.getAverage(); //abs(map(result2.getAverage(), 20, 1900, 0, 250)); // /10;

    //double distance1 = abs(map(result1.getAverage(), 20, 1900, 0, 250)); //abs(map(result1.getAverage(), 20, 600, 0, 240));
    //double distance2 = abs(map(result1.getAverage(), 20, 1900, 0, 250)); //abs(map(result2.getAverage(), 20, 600, 0, 240));

    //double distance1 = abs(map(result1.getAverage(), 20, 700, 0, 240)); //abs(map(result1.getAverage(), 20, 600, 0, 240));
    //double distance2 = abs(map(result1.getAverage(), 20, 700, 0, 240)); //abs(map(result2.getAverage(), 20, 600, 0, 240));
        
    cells_in[input_A_toCell] = distance1/100; /*distance1/100;*/  Serial.print(" A= "); Serial.print(distance1); Serial.print(" Cell= "); Serial.print(cells_in[input_A_toCell],5);
    cells_in[input_B_toCell] = distance2/100; /*distance2/100;*/  Serial.print(" B= "); Serial.print(distance2); Serial.print(" Cell= "); Serial.println(cells_in[input_B_toCell],5);
  }
  else
  {
    goto remeasure;   
  }
    
}

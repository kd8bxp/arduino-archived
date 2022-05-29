void biasrandom()
{
  randomSeed(analogRead(3)); //srand(time(0)); 
  double upper = 900;
  double lower = 0;     
  double tmp = double(random(100))/100;
  bias = 2.0 * (tmp - 0.5 ) * InitWeights;   
}

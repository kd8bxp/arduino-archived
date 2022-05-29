void drive()  //  start driving
{
  int cal = 5;
  switch(out)
  {
    case 0: digitalWrite(motA_1, LOW); digitalWrite(motA_2, HIGH);  digitalWrite(motB_1, HIGH); digitalWrite(motB_2, LOW);ledcWrite(Channel_0, 210+battery+cal); ledcWrite(Channel_1, 175+battery); delay(820);   break;
    case 1: digitalWrite(motA_1, HIGH); digitalWrite(motA_2, LOW);  digitalWrite(motB_1, HIGH); digitalWrite(motB_2, LOW);ledcWrite(Channel_0, 0+battery+cal); ledcWrite(Channel_1, 195+battery);   break;
    case 2: digitalWrite(motA_1, HIGH); digitalWrite(motA_2, LOW);  digitalWrite(motB_1, HIGH); digitalWrite(motB_2, LOW);ledcWrite(Channel_0, 210+battery+cal); ledcWrite(Channel_1, 210+battery);   break;
    case 3: digitalWrite(motA_1, HIGH); digitalWrite(motA_2, LOW);  digitalWrite(motB_1, HIGH); digitalWrite(motB_2, LOW);ledcWrite(Channel_0, 195+battery+cal); ledcWrite(Channel_1, 0+battery);  break;
    case 4: digitalWrite(motA_1, HIGH); digitalWrite(motA_2, LOW);  digitalWrite(motB_1, LOW); digitalWrite(motB_2, HIGH);ledcWrite(Channel_0, 175+battery+cal); ledcWrite(Channel_1, 210+battery); delay(820);  break;
    //case 5: digitalWrite(motA_1, LOW); digitalWrite(motA_2, HIGH);  digitalWrite(motB_1, LOW); digitalWrite(motB_2, HIGH);ledcWrite(Channel_0, 170); ledcWrite(Channel_1, 170);   break;   
  }
}

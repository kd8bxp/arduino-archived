void m1achange() {
  detachInterrupt(M1A);
  snap2 = digitalRead(M1B);
  if (snap2 == 0) {
    // encoder channels are the same, direction is positive
    m1dir = 1;
    m1steps++;
  } else {
    // encoder channels are not the same, direction is negative
   m1dir = -1;
    m1steps--;
  }
  attachInterrupt(M1A, m1achange, RISING);
  }

void m2achange() {
  detachInterrupt(M2A);
  snap3 = digitalRead(M2B);
  if (snap3 == 0) {
    // encoder channels are the same, direction is positive
    m2dir = -1;
    m2steps--;
  } else {
    // encoder channels are not the same, direction is negative
   m2dir = 1;
    m2steps++;
  }
  attachInterrupt(M2A, m2achange, RISING);
  }

/*
 * Gives both a speed (rotation count - 360 equals one full rotations for the Romi motor and wheels)
 * and a direction.
 * Direction 0 (zero) is not moving.
 * 1 (one) is moving forward
 * -1 (negitive one) is moving backward
 * 
 */

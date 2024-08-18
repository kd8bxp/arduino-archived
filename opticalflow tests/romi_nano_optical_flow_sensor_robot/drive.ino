void stop() {
  analogWrite(lpwm, 0);
  analogWrite(rpwm, 0);
  digitalWrite(ldir, 0);
  digitalWrite(rdir, 0);
}

void forward(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, HIGH);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, HIGH);
}

void back(int lspd, int rspd) {
  analogWrite(lpwm, lspd);
  digitalWrite(ldir, LOW);
  analogWrite(rpwm, rspd);
  digitalWrite(rdir, LOW);
}

void displayPkg() {
  
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.drawString(35,13,"In: "); //d1 mini
  display.drawString(50,13,(String)distanceIN);
  display.drawString(35,24,"Ct: ");
  char temp[30];
  snprintf(temp,30,"%d,%d", m1steps,m2steps);
  //display.drawString(50,24, (String)m1steps);
  //display.drawString(52,24, ",");
  //display.drawString(55,24, (String)m2steps);
  display.drawString(50, 24, temp);
  display.drawString(35,35, "Dir: ");
  if (m1dir == 0) {display.drawString(55,35, "S");}
  if (m1dir == -1) {display.drawString(55,35, "R");}
  if (m1dir == 1) {display.drawString(55,35, "F");}
  if (m2dir == 0) {display.drawString(60, 35, ",S");}
  if (m2dir == -1) {display.drawString(60, 35, ",R");}
  if (m2dir == 1) {display.drawString(60, 35, ",F");}
  display.drawString(35, 46, RobotID.substring(5));
  display.display();
}

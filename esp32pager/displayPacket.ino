void displaypacket(){
  
  display.clear();
  display.setColor(WHITE);
  //display.drawStringMaxWidth(0,0,110, pkt);
  display.drawString(0,0, "To:");
  display.drawString(15,0, TO);
  display.drawString(0, 10, "From:");
  display.drawString(27, 10, FROM);
  display.drawStringMaxWidth(0, 20,110, MSG1);
  display.println("");
  display.display();
  delay(5000); //need better way to do this, while delayed can't get new messages
  display.clear();
  display.display();
  if (RTCNT > 0) {
    display.drawString(0,60,"Re-Transmit By ");
    for (int xx=0; xx < RTCNT; xx++) {
      display.drawString(0,10*xx,RTCALL[xx]+" "+RTRSSI[xx]);
    }
    display.display();
    delay(3000); //need better way to do this, while delayed can't get new messages
    display.clear();
    display.display();
  }
  //Display Sending Device Info
  display.drawString(0,0,"DeviceID: " + TXDEVICEID);
  display.drawString(0,10,"Device Type: "+ TYPE);
  display.drawString(0,20,"Grid: "+GRID);
  display.drawString(0,30,"Other: "+OTHER);
}


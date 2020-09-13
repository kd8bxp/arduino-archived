//! Uptime in seconds, correcting for rollover. 
//Gives incorrect information since deep sleep installed. (always says
// zero or null)
  long int uptime(){
  static unsigned long rollover=0;
  static unsigned long lastmillis=millis();

  //Account for rollovers, every ~50 days or so.
  if(lastmillis>millis()){
    rollover+=(lastmillis>>10);
    lastmillis=millis();
  }

  return(rollover+(millis()>>10));
}

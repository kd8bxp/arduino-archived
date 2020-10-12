//Output Error messages (if any) to the Serial console

void inputCheck() {
   if (call == NULL) {Serial.println("NULL Call Invalid"); callPassCheck == false; }

  if (call == "BLT" || call == "CQ" || call == "WX" ) { callPassCheck = true; pass1 = true; pass2 = true; }
  
  if (callPassCheck == false) {
    String tempCall;
    tempCall = call.substring(0,call.indexOf("-"));
    
    
             if (tempCall.length()< 4 || tempCall.length()> 7 ) {Serial.println("Call Must be a max 7 digits plus ssid, or BLT, CQ, WX"); pass1 = false;} else { pass1 = true;}
 
              //Now Check SSID
             String ssid;
             ssid = call.substring(call.indexOf("-")+1);
             if (ssid.length()<2 || ssid.length()>2 && pass1 == true) {Serial.println("SSID must be 2 digits."); pass2 = false; } else { pass2 = true;}
             
}
if (call == msg || msg == NULL) {Serial.println("Message Invalid"); msgPassCheck = false;} else {msgPassCheck = true;}
if (msgPassCheck == true && msg.length()>77) {Serial.println("Message too long, max 77 charaters"); pass3 = false; } else {pass3 = true;}



}

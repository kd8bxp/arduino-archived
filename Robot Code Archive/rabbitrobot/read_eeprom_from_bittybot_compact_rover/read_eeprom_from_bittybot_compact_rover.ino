#include <EEPROM.h>
int WIRE,_L_PIN, _L_PWM, _R_PIN, _R_PWM, _L_EN, _R_EN, _WIRE6;
void setup() {

  Serial.begin(9600);
  String prom = "READ";
   if (prom == "READ") {
    int _check = EEPROM.read(172);
    if (_check == 1) {
      WIRE = EEPROM.read(173);
      _L_PIN = EEPROM.read(174);
      _L_PWM = EEPROM.read(175);
      _R_PIN = EEPROM.read(176);
      _R_PWM = EEPROM.read(177);
      if (WIRE == 6) {
        _L_EN = EEPROM.read(178);
        _R_EN = EEPROM.read(179);
        _WIRE6 = 1;
        
      }} else {
        Serial.println("EEPROM not set!");
        while(1) {}
      }
Serial.print("L PIN: ");
        Serial.println(_L_PIN);
        Serial.print("L PWM: ");
        Serial.println(_L_PWM);
        Serial.print("R PIN: ");
        Serial.println(_R_PIN);
        Serial.print("R PWM: ");
        Serial.println(_R_PWM);
        Serial.print("Check: ");
        Serial.println(_check);
        Serial.print("WIRE: ");
        Serial.print(WIRE);
}
   }
void loop() {
  // put your main code here, to run repeatedly:

}

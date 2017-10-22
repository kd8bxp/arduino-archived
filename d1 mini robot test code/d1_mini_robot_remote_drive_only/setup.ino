void setup() {
  Serial.begin(9600);
  pinMode(M1A, INPUT_PULLUP);
  pinMode(M1B, INPUT_PULLUP);
  pinMode(M2A, INPUT_PULLUP);
  pinMode(M2B, INPUT_PULLUP);
//  attachInterrupt(M1A, m1achange, RISING);
//  attachInterrupt(M2A, m2achange, RISING);
  setup_wifi();
 // WiFiManager wifiManager;
 // wifiManager.autoConnect();  
  Udp.begin(localPort);
  client.setServer(mqtt_server, 1883);
  

}


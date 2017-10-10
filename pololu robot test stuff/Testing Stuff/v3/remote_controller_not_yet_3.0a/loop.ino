void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  toggleSwitch();
  if ( SwitchIN != -999 ) {   switchResults(); }
delay(100);
}

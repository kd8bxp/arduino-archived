void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
  toggleSwitch();
  switchResults();
delay(100);
}

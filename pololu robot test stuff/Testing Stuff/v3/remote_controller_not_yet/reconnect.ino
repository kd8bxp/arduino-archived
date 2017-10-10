void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    snprintf(clientid,16,"RobotRemote%08X",chipid);
    // Attempt to connect
    if (client.connect(clientid)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("robothome", clientid);
      // ... and resubscribe
      client.subscribe("robothome");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      updSender(0, 0, mode);
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    yield();
    stop(); //Stop motors....we lost at least one connection to the network.
      cmd = 0;
    Serial.print("Attempting MQTT connection...");
    snprintf(clientid,16,"RobotID%08X",chipid);
    // Attempt to connect
    if (client.connect(clientid)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
     publishTopicHome();
    } else {
      Udp.flush();
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      Serial.print("Error All Motors Stop....");
      yield();
      stop(); //Stop motors....we lost at least one connection to the network.
      cmd = 0;
      delay(5000); //....robot maybe in trouble.
    }
  }
}

/* Connect and reconnect to MQTT server
 *  Publish the ClientID - and IP address.
 *  The robot uses MQTT topic of "ClientID" to publish updates
 *  Encoder Counts, Ultrasonics, ect
 *  The remote, listens for the "ClientID" and then subscribes to
 *  that ID to listen for the other information.
 */



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    yield();
    Serial.print("Attempting MQTT connection...");
    snprintf(clientid,16,"RobotID%08X",chipid);
    // Attempt to connect
    if (client.connect(clientid)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      StaticJsonBuffer<200> sendHomeJsonBuffer;
JsonObject& sendHomeRoot = sendHomeJsonBuffer.createObject();
sendHomeRoot["robotID"] = clientid;
//Republish ClientID and IP address (probably need a timer here)
//client.publish("robothome", clientid); //publish robot name (This is the topic the robot publishes too)

//Convert IP Address to char array
  String ipaddress = WiFi.localIP().toString();
  char ipchar[ipaddress.length()+1];
  ipaddress.toCharArray(ipchar,ipaddress.length()+1);
 sendHomeRoot["IP"] = ipchar; 
//client.publish("robothome", ipchar); //This is the Robot IP address used for UDP control
char tempArr1[200];
sendHomeRoot.printTo(tempArr1);
client.publish("robothome", tempArr1);
    } else {
      Udp.flush();
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      Serial.print("Error All Motors Stop....");
      yield();
      stop(); //Stop motors....we lost at least one connection to the network.
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



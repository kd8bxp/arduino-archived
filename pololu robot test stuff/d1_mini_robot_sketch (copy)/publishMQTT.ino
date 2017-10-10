void publishMQTT() {

  //Check to see if we are still connected to the MQTT server
if (!client.connected()) {
    reconnect();   }

//Republish ClientID and IP address (probably need a timer here)
client.publish("robothome", clientid); //publish robot name (This is the topic the robot publishes too)

//Convert IP Address to char array
  String ipaddress = WiFi.localIP().toString();
  char ipchar[ipaddress.length()+1];
  ipaddress.toCharArray(ipchar,ipaddress.length()+1);
  
client.publish("robothome", ipchar); //This is the Robot IP address used for UDP control

//Create Array to send....
StaticJsonBuffer<200> sendJsonBuffer;
  JsonObject& sendRoot = sendJsonBuffer.createObject();
sendRoot["RobotID"] = clientid; //chipid;
//Serial.print("ID: ");
//Serial.print(chipid);
//Serial.print("  Hex:");
//Serial.println(chipid, HEX);
sendRoot["distance"] = RangeInInches;
JsonArray& direction = sendRoot.createNestedArray("direction");
direction.add(m1dir);
direction.add(m2dir);
JsonArray& encoder = sendRoot.createNestedArray("encoder");
encoder.add(m1steps);
encoder.add(m2steps);

char tempArr[200];
sendRoot.printTo(tempArr);
client.publish(clientid, tempArr);

}


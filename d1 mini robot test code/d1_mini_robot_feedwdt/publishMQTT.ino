void publishMQTT() {

  //Check to see if we are still connected to the MQTT server
if (!client.connected()) {
    reconnect();   }
    publishTopicHome();
  
  //Create Array to send....
  StaticJsonBuffer<200> sendJsonBuffer;
  JsonObject& sendRoot = sendJsonBuffer.createObject();
  sendRoot["RobotID"] = clientid; //chipid;
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
ESP.wdtFeed();

}

void publishTopicHome() {
  
  StaticJsonBuffer<200> sendHomeJsonBuffer;
  JsonObject& sendHomeRoot = sendHomeJsonBuffer.createObject();
  sendHomeRoot["robotID"] = clientid; //publish robot name (This is the topic the robot publishes too)

  //Convert IP Address to char array
  String ipaddress = WiFi.localIP().toString();
  char ipchar[ipaddress.length()+1];
  ipaddress.toCharArray(ipchar,ipaddress.length()+1);
  sendHomeRoot["IP"] = ipchar; //This is the Robot IP address used for UDP control

  char tempArr1[200];
  sendHomeRoot.printTo(tempArr1);
  client.publish("robothome", tempArr1);

}


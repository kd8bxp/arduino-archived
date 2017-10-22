void publishMQTT() {

  //Check to see if we are still connected to the MQTT server
if (!client.connected()) {
    reconnect();   }
    publishTopicHome();
  
  //Create Array to send....
  StaticJsonBuffer<200> sendJsonBuffer;
  JsonObject& sendRoot = sendJsonBuffer.createObject();
  sendRoot["RobotID"] = clientid; //chipid;
  sendRoot["distance"] = 0;
  JsonArray& direction = sendRoot.createNestedArray("direction");
  direction.add(0);
  direction.add(0);
  JsonArray& encoder = sendRoot.createNestedArray("encoder");
  encoder.add(0);
  encoder.add(0);


sendRoot.printTo(tempArr);
client.publish(clientid, tempArr);
yield();

}

void publishTopicHome() {
  
  StaticJsonBuffer<200> sendHomeJsonBuffer;
  JsonObject& sendHomeRoot = sendHomeJsonBuffer.createObject();
  sendHomeRoot["robotID"] = clientid; //publish robot name (This is the topic the robot publishes too)

  //Convert IP Address to char array
  ipaddress = WiFi.localIP().toString();
  //char ipchar[ipaddress.length()+1]; 
  ipaddress.toCharArray(ipchar,16);
  sendHomeRoot["IP"] = ipchar; //This is the Robot IP address used for UDP control

  //char tempArr1[200];
  sendHomeRoot.printTo(tempArr);
  client.publish("robothome", tempArr);

}


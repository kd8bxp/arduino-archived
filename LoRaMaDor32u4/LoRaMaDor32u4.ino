/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

// Main LoRaMaDoR network class, plus some auxiliary types

#include "Packet.h"
#include "Network.h"
#include "ArduinoBridge.h"
#include "Console.h"
//#include "TCPIP.h"

Ptr<Network> Net;

void setup()
{
	Serial.begin(115200);
	
	Callsign cs = arduino_nvram_callsign_load();
	Net = Ptr<Network>(new Network(cs));
	
	console_setup(Net);
	//wifi_setup(Net);
}

void loop()
{
	//wifi_handle();
	console_handle();
	Net->run_tasks(millis());
}

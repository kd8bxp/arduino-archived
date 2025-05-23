/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

// Abstract class for application protocols

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

//#include <cstddef>
#include <stddef.h>
#include <stdint.h>
//#include <cstdint>
#include "Pointer.h"

class Network;
class Packet;

struct HandlerResponse {
	HandlerResponse();
	HandlerResponse(Ptr<Packet>, bool);
	Ptr<Packet> pkt;
	bool hide_from_user;
};

class Protocol {
public:
	Protocol(Network*);
	virtual HandlerResponse handle(const Packet&);
	virtual Ptr<Packet> modify(const Packet&);
	virtual ~Protocol();
protected:
	Network *net;
	// This class must be new()ed and not fooled around
	Protocol() = delete;
	Protocol(const Protocol&) = delete;
	Protocol(Protocol&&) = delete;
	Protocol& operator=(const Protocol&) = delete;
	Protocol& operator=(Protocol&&) = delete;
};

#endif

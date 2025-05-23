/*
 * LoRaMaDoR (LoRa-based mesh network for hams) project
 * Copyright (c) 2019 PU5EPX
 */

// Serial and telnet console. Intermediates communication between
// CLI and the platform streams (serial, Telnet).

#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "Packet.h"
#include "Network.h"
#include "ArduinoBridge.h"
#include "CLI.h"

class Network;

void console_setup(Ptr<Network> net);
void console_handle();
void console_telnet_enable();
void console_telnet_disable();

// Receive keystrokes from Telnet
void console_telnet_type(char c);

// Goes to Telnet session, or serial
void console_print(const char *);
void console_print(const Buffer &);
void console_print(char);
void console_println(const char *);
void console_println(const Buffer &);
void console_println();

// Goes straight to serial
void serial_print(const char *);
void serial_print(const Buffer &);
void serial_println(const char *);
void serial_println(const Buffer &);

#endif

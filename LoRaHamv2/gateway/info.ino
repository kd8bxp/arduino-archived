/* 
 *  
 * This program is based on work by KK4VCZ (LoRaHam) - it is no longer compatable with that project and has taken on a life of it's own.
 * https://github.com/travisgoodspeed/loraham/
 * 
 * This is a modified version of something that I was working on, and thinking about for the LoRaHam project originally work started in 2017
 * with changes for use with the TTGO ESP32 LoRa boards
 * Modified by LeRoy Miller, KD8BXP
 * Aug 25, 2017 For use with the TTGO WIFI_LoRa_32 Boards
 * https://www.aliexpress.com/item/TTGO-loraSX1278-ESP32-0-96OLED-16-Mt-bytes-128-Mt-bit-433Mhz-for-arduino/32824758955.html
 * 
 * Library Changed to: arduino-LoRa
 * https://github.com/sandeepmistry/arduino-LoRa
 * 
 * Attempt to Tokenized LoRaHam Protocol to use JSON
 * Library: https://github.com/bblanchon/ArduinoJson/
 * 
 * The gateway program was originally converted on Aug 29, 2020, by LeRoy Miller
 * This version of the program is for use with a LoRa32u4 module (or LoRa32u4 II module) Which should work well for "Relay and WIDE" stations
 *
 * This version of the project has made "modules" for easier understanding, each module can be used to create new sketches for other boards.
 * Or to add something to the existing protocols. 
 * 
 * The new protocol sends a JSON string over LoRa. 
 * Currently this supports:
 * TO, FROM, MES (messages), RT (re-transmit counter), COUNT (count is the TX site counter), UPTIME (TX site uptime counter). 
 * 
 * Because this is a JSON string, the order of the above doesn't mater, and other items can easily be added. IE:
 * a WX could added for a weather station, or SENSOR for a station with sensors, or LAT/LON 
 * 
 * All stations can recieve a message to a "BEACON station", "CQ", and "BLT"
 * 
 * Devices:
 * Gateways use a serial port connected to a computer, other software running on the computer is needed. GATEWAYS will recieve all messages, and re-transmit only if they have not already transmitted the message.
 * Depending on the software on the computer, a gateway could post messages to APRS, or better a MQTT broker where remote hams could subscribe to topics to transmit too. (Connecting to a MQTT broker is the prefered method)
 * 
 * Pagers are two way devices (generally ESP32 based, but others could be used) stand-alone devices - Pagers will display CQs, BEACONs and messages directed to them.
 *  they will re-transmit CQ, BEACONS, and messages not directed to them. 
 *  
 * Other types of devices can be added, these can be sensors, wx stations or anything you can think of.
 *  
 *  MQTT Brokers:
 *  Local networks, should run a broker that is bridged to a internet broker. A raspberry pi connected to a gateway device would make an excellent local broker.
 *  Software running the raspberry pi could direct messages as needed. 

Sep 6, 2020 based on APRS Path found here http://wa8lmf.net/DigiPaths/
the network will have a PATH added to the JSON string, RT, COUNT and UPTIME will be removed.
The PATH will start as RELAY, WIDE2-2, this should create 3 hops when a packet is retransmitted.
digipeater, and relay stations will still add their call sign, and the path can be displayed if needed.
We will have PAGER, GATEWAY, and WIDE stations, these will be the rolls of the devices, and will also indicate how they handle the message
Because the network is small a PAGER will be used as a RELAY station, can respond to bluetooth, serial, or wifi input (ESP32 devices with screens are perfect for these kind of devices) LoRa32-OLED, T-BEAM, ect. If the network grows, or if there is a lot of ping/pong happening, PAGER may have RELAY removed and only be used to send or recieve messages.
GATEWAY will also responsed to RELAY, WIDE and will gate traffic to a MQTT broker (ESP32, or another connected device would be the easy way to achive this) GATEWAYS can also responsed to serial input if connected to a computer.
a WIDE device will responsed to RELAY and WIDE. and can response to serial input if connected to a computer. (But think of them as repeaters) LoRa32u4 or devices without screens should work well for these stations. 
Hopefully using a PATH like APRS it will cut down on duplicated messages, duplicate check may need to be added if a lot of duplicate are seen.
 */

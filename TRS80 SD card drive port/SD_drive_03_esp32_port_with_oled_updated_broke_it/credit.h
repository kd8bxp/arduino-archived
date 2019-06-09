/*
 * WiFi SD card drive 
 * 
 * Ported to the ESP32 (TTGO T-Eight Board) by LeRoy Miller, May 2019
 * 
 * based on the following projects: 
 * 
 *  **************************************************************************
 *  **** MyTeSed: T100 SD Card Reader for Arduino Nano and SD Card Reader ****
 *  **** Should work with other rs232 enabled PC's in text mode transfers ****
 *  **** Xmodem for bin files, but this is not working on the Tandy ATM.  ****
 *  **************************************************************************
 *  ** David Stephenson 2017-10-11  **
 *  ** Version 0.03                 **
 *  **********************************
 *  For more information about this project see:
 *  https://www.zx81keyboardadventure.com/2017/08/model-100-micro-sd-card-reader-mytesed.html
 *  https://youtu.be/7AtsHzl9WMc
 *  
 *  And the C64-WiFiModem project which was based on serveral other projects as well.
 *  https://github.com/DrZingo/c64-wifimodem
 *  
   C64-WiFiModem - (c) 2017 my2bits <markus at borgelin.org>
   changes here
   Built hardware based on ESP-12, 3.3V-reg and level shifters.
   Tidy up help, and categorizing
   Added automatic firmware check at startup
   Corrected connectWifi() link check
   Changed MAX_CMD_LENGTH to actual FIFO buffer size
   Fixed help-bug in 9600+ baud
   Corrected setBaudRate(), bauds[] array and sizeof, to error out correct
   Adjusted pinouts for my needs
   Adjusted default servers
   Added modem restart command
   Added OLED display with information
   Added OTA Update
   Added scan for wifi
   Some fixes to Pin polarity, does not work as expected though. Fixes welcome
   Probably added more stuff I forgot to write here.
   Thanks to Paul Richards and Jussi Salin for source code.

   based on
   WiFi SIXFOUR - A virtual WiFi modem based on the ESP 8266 chipset
   Copyright (C) 2016 Paul Rickards <rickards@gmail.com>
   Added EEPROM read/write, status/help pages, busy answering of incoming calls
   uses the readily available Sparkfun ESP8266 WiFi Shield which has 5v level
   shifters and 3.3v voltage regulation present-- easily connect to a C64
   https://www.sparkfun.com/products/13287

   based on
   ESP8266 based virtual modem
   Copyright (C) 2016 Jussi Salin <salinjus@gmail.com>

   https://github.com/jsalin/esp8266_modem

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 /*
  * Changes and Additions:
  * 
  * Added "FILES", "KILL", and "HELP" to SD portion of code
  * Added status on the OLED as to status of SD drive load and save
  * Added xmodem status to OLED
  * Changed "FILES" and "DIR" subroutine back to a previous version that seemed to work better with my hardware
  *  - added file size back for display, plus tabs
  * Changed the filename check and extension check to also include the "/" in file name (needed for my hardware, and SD card) line 356
  *  - change/update code to check if "/" is present if not add it (this will require work)
  *  Line 323 - void OutFileTimeOut() added timeout warning display on OLED
  *  Line 385 - added the new commands that don't require a filename "FILES", "HELP"
  *  Line 402 - IF check "COPY2R" and "CAT" changed to the old method of "LOAD", but I'm not sure I needed to do this - because I missed how LINE 356 was checking the file names
  *  Line 435 added "FILES" and Line 441 added "KILL"
  *  Line 477 added "HELP" and display help on OLED (just list the commands that can be used, but doesn't tell how - simple help)
  *  void Setup added OLED code, added "Intro Screen", SD card check, checked display from Serial to OLED
  *  
  */

copy the hex file from the temporary directory to working directory or better (easier) export compile binary file - issue this command:

avrdude -F -v -patmega128rfa1 -cUSBasp -Uflash:w:hello_world.ino.hex:i 

avrdude: Version 6.3
         Copyright (c) 2000-2005 Brian Dean, http://www.bdmicro.com/
         Copyright (c) 2007-2014 Joerg Wunsch

         System wide configuration file is "/etc/avrdude.conf"
         User configuration file is "/home/lfmiller/.avrduderc"
         User configuration file does not exist or is not a regular file, skipping

         Using Port                    : usb
         Using Programmer              : USBasp
         AVR Part                      : ATmega128RFA1
         Chip Erase delay              : 55000 us
         PAGEL                         : PD7
         BS2                           : PE2
         RESET disposition             : dedicated
         RETRY pulse                   : SCK
         serial program mode           : yes
         parallel program mode         : yes
         Timeout                       : 200
         StabDelay                     : 100
         CmdexeDelay                   : 25
         SyncLoops                     : 32
         ByteDelay                     : 0
         PollIndex                     : 3
         PollValue                     : 0x53
         Memory Detail                 :

                                  Block Poll               Page                       Polled
           Memory Type Mode Delay Size  Indx Paged  Size   Size #Pages MinW  MaxW   ReadBack
           ----------- ---- ----- ----- ---- ------ ------ ---- ------ ----- ----- ---------
           eeprom        65    10     8    0 no       4096    8      0  9000  9000 0x00 0x00
           lfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           hfuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           efuse          0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           lock           0     0     0    0 no          1    0      0  9000  9000 0x00 0x00
           calibration    0     0     0    0 no          1    0      0     0     0 0x00 0x00
           signature      0     0     0    0 no          3    0      0     0     0 0x00 0x00
           flash         65    20   256    0 yes    131072  256    512 50000 50000 0x00 0x00

         Programmer Type : usbasp
         Description     : USBasp, http://www.fischl.de/usbasp/

avrdude: auto set sck period (because given equals null)
avrdude: warning: cannot set sck period. please check for usbasp firmware update.
avrdude: AVR device initialized and ready to accept instructions

Reading | ################################################## | 100% 0.00s

avrdude: Device signature = 0x1ea701 (probably m128rfa1)
avrdude: safemode: hfuse reads as 92
avrdude: safemode: efuse reads as FC
avrdude: NOTE: "flash" memory has been specified, an erase cycle will be performed
         To disable this feature, specify the -D option.
avrdude: erasing chip
avrdude: auto set sck period (because given equals null)
avrdude: warning: cannot set sck period. please check for usbasp firmware update.
avrdude: reading input file "hello_world.ino.hex"
avrdude: writing flash (5092 bytes):

Writing | ################################################## | 100% 2.88s

avrdude: 5092 bytes of flash written
avrdude: verifying flash memory against hello_world.ino.hex:
avrdude: load data flash data from input file hello_world.ino.hex:
avrdude: input file hello_world.ino.hex contains 5092 bytes
avrdude: reading on-chip flash data:

Reading | ################################################## | 100% 2.60s

avrdude: verifying ...
avrdude: 5092 bytes of flash verified

avrdude: safemode: hfuse reads as 92
avrdude: safemode: efuse reads as FC
avrdude: safemode: Fuses OK (E:FC, H:92, L:EE)

avrdude done.  Thank you.


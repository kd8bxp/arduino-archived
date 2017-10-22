/*

Copyright (c) 2017 LeRoy Miller

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>

Change Log:

Oct 10 - Fixed UDP Flooding Issue
        UDP Remote Working.
        Cleaned up and made some comments in code.
        Added more stops to reconnect function (v3.0.2) made it safer.
        fixed date in Change Log. (Oct 7 to Oct 10)
        * Special Note: Do not hold the toggle switch on the remote it will FLOOD and isn't needed.
Oct 10 - Added WifiManager
Oct 10 - v3.0.3 Removed WiFiManager (caused crashes)
         removed some variables not used
         attempted to add protection code (caused WDT, and crashes)
         removed Ultrasonic.h (which was just a simple setup and pin call)
         * Changed D1 Mini microcontroller to another(different) D1 Mini thinking my crashes were a hardware issue (still crashed)
Oct 22 - Added protection mode, for moving forward, appears to be working correctly.
        
ToDo: Last Updated Oct 22, 2017

Add Wifi Manager

* Create a Raspberry PI to run a personal MQTT Broker.

*/

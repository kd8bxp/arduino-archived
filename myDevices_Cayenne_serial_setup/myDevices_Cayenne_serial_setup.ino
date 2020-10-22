
    /*
Cayenne Serial USB Example

This sketch connects to the Cayenne server using an Arduino Serial USB connection
and runs the main communication loop.

The Cayenne Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

For Cayenne Dashboard widgets using digital or analog pins this sketch will automatically
send data on those pins to the Cayenne server. If the widgets use Virtual Pins, data
should be sent to those pins using virtualWrites. Examples for sending and receiving
Virtual Pin data are under the Basics folder.

This requires the use of the Serial USB connection so you cannot use the Serial device for
printing messages. If you need to print you can use SoftwareSerial and connect another device
to read messages via the SoftwareSerial pins.

In order for this to work you must run the connection script on the machine the Arduino is connected to.
The scripts are located under the extras\scripts folder in the main library folder. This redirects the traffic
from the Arduino to the Cayenne server.

Steps:
1. Set the token variable to match the Arduino token from the Dashboard.
2. Compile and upload this sketch.
3. Launch the connection script as described below for Windows or Linux/OSX.

Windows:
  1. Open the Windows command line (cmd.exe)
  2. Navigate to the scripts folder by typing "cd [path]", e.g.  "cd C:\Users\[YourUserName]\Documents\Arduino\libraries\Cayenne\extras\scripts"
  3. Run the script by typing "cayenne-ser.bat -c COM4" (where COM4 is port with your Arduino) and hitting Enter

Linux and OSX:
    ./cayenne-ser.sh (may need to run with sudo)
    
You can specify port, baud rate, and server endpoint like this:
    ./cayenne-ser.sh -c <serial port> -b <baud rate> -s <server address> -p <server port>

    For instance :
      ./cayenne-ser.sh -c /dev/ttyACM0 -b 9600 -s arduino.mydevices.com -p 8442

    Run cayenne-ser.sh -h for more information

    Be sure to select the right serial port (there may be multiple).

ATTENTION!
  Do not use Serial to display any output in this sketch. It will interfere with the Serial
  USB connection. When uploading sketches the Arduino IDE may complain with "programmer is
  not responding" or "Access is denied." You will need to terminate the connection script
  before uploading new sketches since it blocks access to the Serial port. Also make sure 
  the Serial Monitor is disabled in the IDE since that can prevent the Arduino from 
  connecting to the Windows/Linux/OSX machine. If you use Visual Micro for Visual Studio make
  sure Automatic Debugging is disabled. Otherwise the Serial Monitor can interfere with the
  Serial connection.
*/

#include <CayenneSerial.h>

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "";

void setup()
{
  //Baud rate can be specified by calling Cayenne.begin(token, 9600);
  Cayenne.begin(token);
}

void loop()
{
  Cayenne.run();
}


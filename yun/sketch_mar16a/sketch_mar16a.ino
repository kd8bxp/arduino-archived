/*
  SD card datalogger to Dropbox
 
 This example  shows how to log data from three analog sensors 
 to an SD card mounted on the Arduino Yún using the Bridge library.
 	
 The circuit:
 * analog sensors on analog pins 0, 1 and 2
 * SD card attached to SD card slot of the Arduino Yún
 
 You can remove the SD card while the Linux and the 
 sketch are running but be careful not to remove it while
 the system is writing to it.

 You need to create a Dropbox app and get an API key and secret for the Python script called by this sketch. The script should be situated at "/mnt/sda1/LogToDropbox"
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 adapted to the Yún Bridge library 20 Jun 2013
 by Federico Vanzati
 modified  21 Jun 2013
 by Tom Igoe
 modified 13 May 2014
 by Ankit Daftery

 This example code is in the public domain.
 
 	 
 */

#include <FileIO.h>
#include <Process.h>

void setup() {
  // Initialize the Bridge and the Serial
  Bridge.begin();
  Serial.begin(9600);
  FileSystem.begin();

  while(!Serial);  // wait for Serial port to connect.
  Serial.println("Filesystem datalogger\n");
}


void loop () {
  // make a string that start with a timestamp for assembling the data to log:
  String dataString;
  dataString += getTimeStamp();
  dataString += " = ";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";  // separate the values with a comma
    }
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // The FileSystem card is mounted at the following "/mnt/FileSystema1"
  File dataFile = FileSystem.open("/mnt/sda1/datalog.txt", FILE_APPEND);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
  Process upload;
  upload.runShellCommand("python /mnt/sda1/YunLogToDropbox/log.py /mnt/sda1/YunLogToDropbox/datalog.txt");  // upload the file we just wrote to to Dropbox
 
 while(upload.available()) {
  char c=upload.read();
  Serial.print(c);  // Display the response
}


  delay(15000);

}

// This function return a string with the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time 
  // in different formats depending on the additional parameter 
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
  //             T for the time hh:mm:ss    
  time.run();  // run the command

  // read the output of the command
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }

  return result;
}

Instructions for running the ADNS3080ImageGrabber.py

0. Connect your ADNS3080 (DIYDrones) sensor to your Arduino:

 Arduino -> ADNS3080
--------    --------
    5V   ->  5V
    GND  ->  GND
    D9   ->  RST (optional)
    D10  ->  NCS
    D11  ->  MOSI
    D12  ->  MISO
    D13  ->  SCLK
    N/C  ->  NPD, LED, 3V

1. Open ADNS3080Demo.pde with Arduino IDE, compile and upload to your Arduino
2. Install Python 2.7 from http://www.python.org/getit/
3. Start the Python IDLE editor
4. Open ADNS3080ImageGrabber.py from IDLE
5. Select Run->Run Module - the Python Shell and then ADNS3080ImageGrabber applications should appear
6. On the ADNS3080ImageGrabber screen, change the default com port, "COM8" to the correct port (sorry, doesn't auto detect)
7. Wait a few moments, then push the Send button to start/stop grabbing images from the sensor (a new image should appear every 2 seconds)

Note: you should see the the ADNS3080Demo's menu and any errors appear in the Python Shell
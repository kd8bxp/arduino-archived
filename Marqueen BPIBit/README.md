This is an attempt to get the original (DF Robot v2) Maqueen robot working with the BPI:Bit ESP32 (original BPI Bit board).

this is mostly test code, it is no way near complete, but was used to test various sensors on board the robot.

What works:
-----------

Motors drive thou I never finished porting all the code over.
Front RED Leds work
IR remote control - at least recieves codes from a remote (testing staged - and never really finished)
Using neopixel bus library the 4 leds on the bottom work.
Sound/Speaker work
I2C should work (The motors are I2C) There is a I2C pin header on the robot, I haven't tried it thou.

What doesn't work:
------------------

Line Follow sensors (either my maqueen has bad sensors, or the BPI:Bit destroyed them, or????)
 --I even attempted to get them working using a Micro:bit, and couldn't so I suspect they went bad (But how?)
  
Ultrasonic ping senors, doesn't work - I did spend some time on this problem, and it might be a limitation of the BPI:Bit not being able to get a digital read on the echo pin. It could also be that my sensors all need 5v and the robot doesn't give enough power (?)  - I have had problems with these sensors and the esp8266 in the past and it was power related. (So manybe power?)
Back in the day, Radio Shack did make 3 pin, 3.3v (Friendly) ultra sonic sensor (these are not the same as the ultra sonic sensors that parallax sold.) When I wire one of those up to the echo pin port, I can't get any vaule to return (Leading me to believe this is a limitation of the BPI:Bit). However when connected to the transmit pin I am able to get really good readings.
I also attempted to short the one of the 4 pin ultrasonics I have, and couldn't get it to work (It does work like this on a 5v microcontroller thou) - which also leads me to believe that it's a power issue
So maybe there are two issues with ultrasonics the BPI:bit and the robot.

The BPI:Bit needs pin 2 to be high to use the LED matrix, this causes the right LED to stay on (it's on pin 2). This is anonying to say the least. 
It also appears that I can't use both the BPI:bit on board LED matrix, and the 4 LEDs on the robot at the same time....thou I didn't fully expore why or options (the sketch keep crashing) to fix the problem.
(For some reason I couldn't get Adafruit Neopixel Library or FastLED to work with the 4 leds on board the robot - but to my surprise Neopixel BUS did work. I never explored why this is either).

I've stopped working on this (at least for now)
(gone to the big archive in the cloud)

# ESP8266 Relay Control

Using Google Assistant with Dweet.io or Shiftr.io, IFTTT and Webhooks.
Currently there is not a sketch that works with Shiftr.io. 
Shiftr.io take a little more to setup - Shiftr provides and more or less easy way to make a Web POST request, that request is then sent to the Shiftr.io MQTT broker. Making it easy enough to create a webhook.  
Dweet.io is dead simple messaging for devices - think twitter for your device. You don't even need an account to use it.  The down side to this is it's not real secure, So I wouldn't use this for a garage door, or something like that - but turning lights on or off should be ok.  

## Hardware Used

ESP01-S Relay board (Make sure it has the ESP01-S, the ESP01 doesn't work with these relays).  
Alterntive - a Wemos D1 Mini, with Relay shield.  

For the Status display - a D1 Mini and D1 Mini OLED Shield was used.  
(The Status display is a bonus, it's not needed, and I'm not 100% happy with how it displays relay status anyway, but included because.)  


## More Information

I have IFTTT setup so that I can say "OK Google", "test relay on", google will say "Test relay on" and send a message to Dweet.io, in turn the relay is waiting for a message, and turns on the relay.  

You can also say, "test relay off", "test relay on", "test relay status".  
"test relay" can be changed to anything you want, leave the $ sign this is a variable.  


I also added a IFTTT for help, just in case I forget what I can say - I just have to remember how to say the help phrase.  
"Help Relay" or "What can I say relay".   
IFTTT still needs an action, so I setup a webhook that goes no where - IFTTT was happy, and this worked (much to my surprise).  

## Software

Libraries used:  
Dweet - https://github.com/quentinpigne/arduino-dweet.io  
WiFiManager - https://github.com/tzapu/WiFiManager  
Adafruit D1 SSD1306 - https://github.com/kd8bxp/mcauser-64x48-OLED-SSD1306-library  (Please see my README2.md file, it explains this isn't mine, and I made a couple of changes so that I could keep the orginal Adafruit SSD1306 and use this one as well).  
ArduinoJson - https://github.com/bblanchon/ArduinoJson  

dweet status display is the sketch to load on the D1 Mini with OLED.  
dweet with json relay control2 this is the sketch that goes on the relay board (or D1 mini with relay).  
There are a couple of things that you can change or need to be setup here. Change thingName to whatever you want to call your relay.  
And change relayPin to the pin that your relay is connected too.  
For the D1 Mini that is D1, for the ESP01-S that would be Pin 0  


## Usage

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## Support Me

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  
https://www.paypal.me/KD8BXP  

## Other Projects

https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  


## Credits

Copyright (c) 2019 LeRoy Miller

## License

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

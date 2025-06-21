# WeMos D1 Mini + Nokia 5110 PCD8544 LCD Module

The D1 mini board:
http://wemos.cc

Tool used to convert .bmp to byte arrays:
http://javl.github.io/image2cpp/

## Materials:
* WeMos D1 Mini
* Nokia 5110 PCD8544 84x48 monochrome LCD
* 8 jumper wires
* breadboard

## Dependencies:

Add these to your Arduino IDE using Manage Libraries.

* [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
* [Adafruit PCD8544 Nokia 5110 LCD library](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library)
  * There is a pull request that adds ESP8266 support:
  * [adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library#27](https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library/pull/27)
  * If this has not been merged yet, you will need to manually copy the changes across

## Breadboard connections:

WeMos D1 Mini | Nokia 5110 PCD8544 LCD | Description
------------- | ---------------------- | ---------------------------------------------
D2 (GPIO4)    | 0 RST                  | Output from ESP to reset display
D1 (GPIO5)    | 1 CE                   | Output from ESP to chip select/enable display
D6 (GPIO12)   | 2 DC                   | Output from display data/command to ESP
D7 (GPIO13)   | 3 Din                  | Output from ESP SPI MOSI to display data input
D5 (GPIO14)   | 4 Clk                  | Output from ESP SPI clock
3V3           | 5 Vcc                  | 3.3V from ESP to display
D0 (GPIO16)   | 6 BL                   | 3.3V to turn backlight on, or PWM
G             | 7 Gnd                  | Ground

These connections are common to all examples.

The backlight (BL) is optional.
If you would like full brightness all the time, connect BL to 3V3 and save D0 for something else.

## Byte Arrays

I used this tool to convert bitmap images into byte arrays.
http://javl.github.io/image2cpp/

The Adafruit library `drawBitmap()` expects steam of bytes which are drawn horizontally. The first `0xFF` in the array represents the first 8x pixels in the top left corner moving to the right.

Each rows last byte needs to be padded with least significant bits to make it a full byte.

I expected a 5x2 black square image would be exactly 10 bits in the array, however, it appears you need 16 bits with 3 padding bits at the end of each row.

The padding least significant bits in the last byte per row are seemingly ignored.

```
// 5x2 black rectangle
ABCDE
FGHIJ

// last byte on each row zero padded
ABCDE000
FGHIJ000

// as bits
11111000
11111000

// as bytes
0B11111000, 0B11111000
0xF8, 0xF8
```

```
// 9x2 black rectangle
ABCDEFGHI
JKLMNOPQR

// last byte on each row zero padded
ABCDEFGH I0000000
JKLMNOPQ R0000000

// as bits
11111111 10000000
11111111 10000000

// as bytes
0B11111111, 0B10000000, 0B11111111, 0B10000000
0XFF, 0x80, 0XFF, 0x80
```

To make the image2cpp tool output an Adafruit compatible byte array, you need to upload an image then round up the width to make it divisble by 8. eg. Upload a 84x48 image, then adjust the width to 88x48 then generate the bytes. Upload a 9x2 image and you need to change the size to 16x2 then generate.

The internal buffer in the library stores the bits in 6 rows of 84x columns of 8 bits. Completely different to what `drapBitmap()` accepts!
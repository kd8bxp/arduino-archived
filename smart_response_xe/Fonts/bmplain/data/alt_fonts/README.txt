information found here: https://jared.geek.nz/2014/jan/custom-fonts-for-microcontrollers
converted for use with the smart response xe (IE: made one big array vs a array for each letter)
The font convert python script he provided so far I haven't been able to get it to work correctly, but it may contain what is needed for converting fonts.


Large & Normal font - are 3 sets of 3 pixels - Commo-Monospaced_font (8 bit font, not 9 bit but appears to work)
Array is called: ucFont[]


Small and Medium font -  2 sets of 3 pixels - 6 bit font - Blokus_font (appears to be working) 
Array is called: ucSmallFont[]

Some of these work better then others, give them a try (variable space fonts appear to be the biggest issue. monospaced seem to work ok)


These fonts were found on the website above, along with the python script to convert them.

Useage is simple, copy the array from the "font_name.h" into the font.h removing the array that is already in place.
Limitation: only 2 fonts - they are different sizes, (Large & Normal, or Small and Medium)

Personally (so far) I like BMSPA_font.h (8bit font), and BMPlain_font.h (6bit), 5x5_font.h (6bit), 

A lot of the 6bit fonts are variable spaced, so you need to try to adjust the size yourself, or live with them being not quite right.

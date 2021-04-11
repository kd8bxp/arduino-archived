TFT fonts need to be converted to BDF format. This is easy using OTF2BDF  
otf2bdf spaceage.ttf -o spaceage.bdf  

Install (and make) bdfe from https://github.com/achilikin/bdfe  
use bdfe to convert a BDF file to a header file (or at least almost an array)  
./bdfe -h -v -l spaceage.bdf > spaceage_other.h  

The new file will need to have a line like this added:  
const unsigned char fivexfive[] PROGMEM = {  
and of course the closing brace at the end of the array.  

This should be able to be used like any other file, depending on how many bits (6 or 8) will depend on if you will use large/normal or small/medium.  

The files font6x8.h and font88.h were also included in the above repository as of right now they have not been converted, tested, or used.  

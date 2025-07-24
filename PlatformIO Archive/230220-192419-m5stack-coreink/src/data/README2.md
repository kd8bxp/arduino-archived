Original Sketch by Robson Couto 2019 https://github.com/robsoncouto/arduino-songs 
Converted for use with the M5Stack Core Ink by LeRoy Miller Jan 2021

All of these images, were resized, and converted to an array using converted to array using https://www.digole.com/tools/PicturetoC_Hex_converter.php The default code format of HEX:0x and used for Black/White, for "DIM" 
some of the images had to converted, resized, grayscaled, and edge detected before they would display (either somewhat correctly, or at all) Most of the images are 200x200 but see the header file for specifics.

sonic(grayscale).h is the image of sonic with the wings behind him (sonic-the-hedgehog-gs.jpg). (looks ok, not great)
sonic(gsoutline).h is the image of imagegs(copy).jpg worked ok

sonic(head).h is a color image (sonic-head.jpg) resized to 200x121, it is just an outline and worked ok, there was no effects or changes other then size.

sonic(head2black).h and sonic(head2white).h are really the same file, just inverse of each other. Black is a black background with sonic as a white outline, while white is a white background with sonic as a black outline. (I liked how this one looked, and used it as the default sonic.h file)

sonic(imagegs).h is the imagesgs.jpg which worked pretty good.
The color images, and the grayscale images show the progression to get to what is  displayed 

sonic.h is the sonichead2white.jpg which worked really well. It's a very simple outline of sonics head.

To use any of these renamed to "sonic.h" and moved to the root directory of this sketch. But remember to save the original "sonic.h" that is already there. (otherwise you'll lose it)


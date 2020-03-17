/*
   Moving Square Example

   This example shows how to handle simple movement of an object with
   the TVOut code and the Video Game Shield.
   
   One of the things that helped me understand the type of graphics
   the TVOut library uses was to try something like this program.
   
   To make the movement of your object look right, you have to handle
   the process of erasing and drawing an object on the screen.  Otherwise
   it leaves the old pixels on the screen.  You can expirament with this
   by commenting out the line that handles the erasing of the square
   (and turn the moving square into a snake).
   
   -----
   2012 - trodoss
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
 */
 
#include <stdlib.h> 
#include <i2cmaster.h>
#include <nunchuck.h>
#include <TVout.h>

#define SCREENWIDTH     128
#define SCREENHEIGHT    96

#define DIRECTION_UP    0
#define DIRECTION_DOWN  1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT 3

TVout TV;
Nunchuck player1;
char square_x;  //represents the square's X position
char square_y;  //represents the square's Y position

void setup()
{
  TV.begin(_NTSC, SCREENWIDTH, SCREENHEIGHT);

  //start the input on controller 1
  player1.begin(0);
  
  //set the initial x and y position of the square
  square_x = 62;
  square_y = 46;
  
  //draw the square initially on the screen
  TV.draw_rect(square_x, square_y, 4, 4, 1, 1);
}


void loop()
{  
    //the loop waits for a directon to be pressed
	//on the joystick.  Once a direction has been
	//pressed, move the square in that direction.
	
    player1.update();
       
    if (player1.joy_up())
	{
	   move_square(DIRECTION_UP);
	}
	
    if (player1.joy_down())
	{
       move_square(DIRECTION_DOWN);
    }
	
    if (player1.joy_right())
	{
       move_square(DIRECTION_RIGHT);
    }
	
    if (player1.joy_left())
    {
       move_square(DIRECTION_LEFT);
    }
}

void move_square(char direction)
{
   //erase the old square's position
   TV.draw_rect(square_x, square_y, 4, 4, 0, 0);
   
   /* determine what to do when moving in a direction;
      the 'switch' statement determines what case we are in
      when moving in a particular direction, first check that
      the movement will not take the square 'out of bounds'
      (off the screen).  If it would, then don't move in that
      direction.
   */
   
   switch (direction)
   {
      case DIRECTION_UP:
	    if (square_y > 4) square_y --;
		break;
		
      case DIRECTION_DOWN:
	    if (square_y < 90) square_y++;
		break;
	
      case DIRECTION_LEFT:
	    if (square_x > 4) square_x--;
		break;	
		
      case DIRECTION_RIGHT:
	    if (square_x < 122) square_x++;
		break;			
   }
   
  //draw the square back on the screen in the new position
  TV.draw_rect(square_x, square_y, 4, 4, 1, 1);    
}


#include <avr/pgmspace.h>
#include "gamby_emulation.h"
#include "map.h"
#include "item.h"
#include "item_bitmap.h"
#include "helper.h"

extern GambyEmulation gamby;

#define ITEM_STEP_LENGTH  4

//For weapons, the "step" is being re-used to determine facing
//when drawing the item on the screen.

MapElement item_move(MapElement element)
{
  
   if (element.state > STATE_HIDDEN) 
   { 
    //test to see if the item has passed out of view  
    if (map_view_test(element.x, element.y))
    {
    
      switch (element.state)
      {
        case STATE_VISIBLE:
  	   element.step = 0;
  	   break;
  	   
  	   case STATE_MOVE_UP:
           element.state = STATE_HIDDEN;
  	   element.step = WEAPON_FACING_UP;
           if (map_check_move(element.x, element.y - ITEM_STEP_LENGTH) == 0)
           {
              element.y -= ITEM_STEP_LENGTH;
              element.state = STATE_MOVE_UP;
           }
  	   break;
  	   
  	  case STATE_MOVE_DOWN:   
          element.state = STATE_HIDDEN;
  	  element.step = WEAPON_FACING_DOWN;
               if (map_check_move(element.x, element.y + (8 + ITEM_STEP_LENGTH)) == 0)
               {
                 element.y += ITEM_STEP_LENGTH;
                 element.state = STATE_MOVE_DOWN;
               }
  	   break;	   
  
  	  case STATE_MOVE_LEFT:
          element.state = STATE_HIDDEN;
  	  element.step = WEAPON_FACING_LEFT;
               if (map_check_move(element.x - ITEM_STEP_LENGTH, element.y) == 0)
               {
                 element.x -= ITEM_STEP_LENGTH;
                 element.state = STATE_MOVE_LEFT;
               }
  	   break;	   
  	   
  	  case STATE_MOVE_RIGHT:
           element.state = STATE_HIDDEN;
  	   element.step = WEAPON_FACING_RIGHT;
               if (map_check_move(element.x + (8 + ITEM_STEP_LENGTH), element.y) == 0)
               {
                element.x += ITEM_STEP_LENGTH;
                element.state = STATE_MOVE_RIGHT;
               }
  	   break;		      
      }
  
      //draw item on the screen
      if (element.state > STATE_HIDDEN) {
        helper_draw_sprite(element.x-map_view_x(), element.y-map_view_y(), item_bitmap, (element.type - 49) + element.step);
      }
    } else {
      if (element.type >= ITEM_WEAPON) element.state = STATE_HIDDEN;
    }
  }
  return element;
}

MapElement item_hit(MapElement element)
{ 
  element.state = STATE_HIDDEN;
  return element;
}

void item_draw(char x, char y, char id)
{
  helper_draw_sprite(x, y, item_bitmap, id);
}

short item_get_x(char type)
{
  MapElement findItem = map_get_element_type(type);
  return findItem.x;
}

short item_get_y(char type)
{
  MapElement findItem = map_get_element_type(type);
  return findItem.y;
}



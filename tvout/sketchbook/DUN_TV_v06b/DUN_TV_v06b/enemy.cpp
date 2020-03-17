#include <avr/pgmspace.h>
#include "gamby_emulation.h"
#include "map.h"
#include "enemy.h"
#include "hero.h"
#include "enemy_bitmap.h"
#include "helper.h"
#include "sound.h"


extern GambyEmulation gamby;
extern byte items;
extern unsigned char experience;
extern unsigned char life;
extern unsigned int score;

MapElement enemy_move(MapElement element)
{

  if (element.state > STATE_HIDDEN)
  {  
     switch (element.step_dir)
     {
       case ENEMY_STEP_UP:
          element.step++;
          if (element.step > 2)
          {
            element.step_dir = ENEMY_STEP_DOWN;
            element.step = 1;
          }
          break;
          
          
      case ENEMY_STEP_DOWN:
        element.step--;
        if (element.step == 0)
        {
          element.step_dir = ENEMY_STEP_UP;
        }
        break;
     }
     
    if (map_view_test(element.x, element.y))
    {
      //if you are invisible, monsters don't react to you
      if (bitRead(items,4) == 0)
      {
        if (bitRead(items,7) == 1)
        {
          //Someone stinks!  Run, monsters, RUN!!!!!
          //chase logic
          if (get_hero_x() < element.x)
          {
            if (map_test_elements(element.id, (element.x + 8) + HERO_STEP_LENGTH, element.y + 4) == 0)
            {
              element.state = STATE_MOVE_LEFT;
              element.x += HERO_STEP_LENGTH;
            }
          } else {
            if (get_hero_x() > element.x)
            {
              if (map_test_elements(element.id, element.x - HERO_STEP_LENGTH, element.y + 4) == 0)
              {
                element.state = STATE_MOVE_RIGHT;
                element.x -= HERO_STEP_LENGTH;
              }
            }
          }
      
          if (get_hero_y() < element.y)
          {
            if (map_test_elements(element.id, element.x + 4, (element.y + 8)+ HERO_STEP_LENGTH) == 0)
            {
              element.state = STATE_MOVE_UP;
              element.y += HERO_STEP_LENGTH;
            }
          } else {
            if (get_hero_y() > element.y)
            {
              if (map_test_elements(element.id, element.x+4, element.y - HERO_STEP_LENGTH) == 0)
              {
                element.state = STATE_MOVE_DOWN;
                element.y -= HERO_STEP_LENGTH;
              }
            }
          }
          
        } else { 
          //chase logic
          if (element.x > get_hero_x())
          {
            if (map_test_elements(element.id, element.x - HERO_STEP_LENGTH, element.y + 4) == 0)
            {
              element.state = STATE_MOVE_LEFT;
              element.x -= HERO_STEP_LENGTH;
            }
          } else {
            if (element.x < get_hero_x())
            {
              if (map_test_elements(element.id, (element.x + 8)+ HERO_STEP_LENGTH, element.y + 4) == 0)
              {
                element.state = STATE_MOVE_RIGHT;
                element.x += HERO_STEP_LENGTH;
              }
            }
          }
      
          if (element.y > get_hero_y())
          {
            if (map_test_elements(element.id, element.x + 4, element.y - HERO_STEP_LENGTH) == 0)
            {
              element.state = STATE_MOVE_UP;
              element.y -= HERO_STEP_LENGTH;
            }
          } else {
            if (element.y < get_hero_y())
            {
              if (map_test_elements(element.id, element.x+4, (element.y + 8) + HERO_STEP_LENGTH) == 0)
              {
                element.state = STATE_MOVE_DOWN;
                element.y += HERO_STEP_LENGTH;
              }
            }
          }
        }
      }
      helper_draw_sprite(element.x-map_view_x(), element.y-map_view_y(), enemy_bitmap, (element.type * ENEMY_OFFSET) + (element.state - 2) + element.step);
    }
  }
  return element;
}

MapElement enemy_hit(MapElement element)
{
  sound_kill();
  element.state = STATE_HIDDEN;
  //add in experience;  every 20 XP gives 20 life
  experience++;
  score++;
  if (experience % 20 == 0) 
  {
    if (life + 20 < 255) 
	{
	   life+=20;
	} else {
	   life = 255;
	}
  }
  return element;
}

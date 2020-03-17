#include "Arduino.h" 
#include <avr/pgmspace.h>
#include "gamby_emulation.h"
#include "map.h"
#include "hero.h"
#include "hero_bitmap.h"
#include "helper.h"
#include "sound.h"

short hero_x;
short hero_y;
char hero_facing;
char hero_step_dir;
char hero_step;
char hero_type;

extern GambyEmulation gamby;
extern byte items;
extern unsigned char life;
extern unsigned int score;
extern char game_state;

void hero_init(char type)
{
  hero_type = type;
  hero_x = 8;
  hero_y = 8;
  hero_step = 0;
  hero_step_dir = HERO_STEP_UP;
  hero_facing = HERO_FACING_RIGHT;
}

void hero_next_level()
{
  hero_x = 8;
  hero_y = 8;
  hero_step = 0;
  hero_step_dir = HERO_STEP_UP;
  hero_facing = HERO_FACING_RIGHT;  

  //reset items for the next level
  items = 0;
}

void hero_move(char facing)
{
  if (facing != hero_facing)
  {
    hero_step = 0;
    hero_step_dir = HERO_STEP_UP;
  } 
  else {
    switch (hero_step_dir)
    {
    case HERO_STEP_UP:
      hero_step++;
      if (hero_step > 2)
      {
        hero_step_dir = HERO_STEP_DOWN;
        hero_step = 1;
      }
      break;


    case HERO_STEP_DOWN:
      hero_step--;
      if (hero_step == 0)
      {
        hero_step_dir = HERO_STEP_UP;
      }
      break;
    }
  }

  hero_facing = facing;

  switch (facing)
  {
  case HERO_FACING_UP:
    if (map_check_move(hero_x+4, hero_y - HERO_STEP_LENGTH) == 0)
    {
      if (hero_y > 0) hero_y -= HERO_STEP_LENGTH;
      if ((map_view_y() > 0) && (hero_y < 216))
      {
        map_move_view(DIRECTION_UP);    
      }                   
    }
    break;

  case HERO_FACING_DOWN:
    if (map_check_move(hero_x+4, (hero_y + 8) + HERO_STEP_LENGTH) == 0)
    {
      if (hero_y < 255) hero_y += HERO_STEP_LENGTH;
      if (hero_y - map_view_y() > 23)
      {
        map_move_view(DIRECTION_DOWN);    
      }                                     
    }  
    break;

  case HERO_FACING_LEFT:
    if (map_check_move(hero_x - HERO_STEP_LENGTH, hero_y+4) == 0)
    {
      if (hero_x > 0) hero_x -= HERO_STEP_LENGTH;
      if ((map_view_x() > 0) && (hero_x < 200))
      {
        map_move_view(DIRECTION_LEFT);    
      } 
    }
    break;	

  case HERO_FACING_RIGHT:
    if (map_check_move((hero_x + 8)+ HERO_STEP_LENGTH, hero_y+4) == 0)
    { 
      if (hero_x < 255) hero_x += HERO_STEP_LENGTH;         
      if (hero_x - map_view_x() > 37)
      {
        map_move_view(DIRECTION_RIGHT);    
      }                  
    }
    break;			
  }  
}

void hero_use_weapon()
{
  sound_fire();
  
  MapElement weapon = map_get_element(0);
  if (weapon.state == STATE_HIDDEN)
  {
    switch (hero_facing)
    {
    case HERO_FACING_DOWN:
      weapon.state = STATE_MOVE_DOWN;
      weapon.x = hero_x;
      weapon.y = hero_y + 8;
      break;


    case HERO_FACING_UP:
      weapon.state = STATE_MOVE_UP;      
      weapon.x = hero_x;
      weapon.y = hero_y - 8;
      break;

    case HERO_FACING_LEFT:
      weapon.state = STATE_MOVE_LEFT;      
      weapon.x = hero_x - 8;
      weapon.y = hero_y;
      break;

    case HERO_FACING_RIGHT:
      weapon.state = STATE_MOVE_RIGHT;      
      weapon.x = hero_x + 8;
      weapon.y = hero_y;
      break;
    }

    map_update_element(weapon, 0);
  }
}

void hero_draw()
{
  helper_draw_sprite(hero_x-map_view_x(), hero_y-map_view_y(), character_bitmap, (hero_type * HERO_OFFSET) + hero_facing + hero_step);
}

void hero_draw_pic(char x, char y)
{
  helper_draw_sprite(x, y, character_bitmap, (hero_type * HERO_OFFSET));
}

short get_hero_x()
{
  return hero_x;
}

short get_hero_y()
{
  return hero_y;
}

char get_hero_type()
{
  return hero_type;
}

MapElement hero_hit(MapElement element)
{
  if (element.type <= 48)
  {
    sound_hit();
    element.state = STATE_HIDDEN;
    //if the hero does not have armor they can take damage
    if (bitRead(items,5) == 0)
    {
      char test = 5;
      if (element.type > 0) test = (element.type * 5);

      if (life - test > 0)
      {
        life -= test;  
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }
    }
  } 
  else {
    MapElement temp;
    switch (element.type)
    {

    case ITEM_EXIT:
      game_state = 13;  //STATE_GAME_SAVE
      //receive some points
      score +=5;
      break;

    case ITEM_KEY:
      //show exit
      temp = map_get_element(1);
      temp.state = STATE_VISIBLE;
      map_update_element(temp, 1);

      //hide the key
      element.state = STATE_HIDDEN;

      //add key to inventory
      bitSet(items,2);

      //receive some points
      score +=1;
      sound_item();
      break;

    case ITEM_MAP:
      //add map to inventory
      bitSet(items,0);

      //hide the map
      element.state = STATE_HIDDEN;

      //receive some points
      score +=1;
      sound_item();
      break;        

    case ITEM_COMPASS:
      //add compass to inventory
      bitSet(items,1);

      //hide the compass
      element.state = STATE_HIDDEN;

      //receive some points
      score +=1;
      sound_item();
      break;  

    case ITEM_INVISIBLE:
      bitSet(items,4);
      element.state = STATE_HIDDEN;
      sound_item();
      break;

    case ITEM_ARMOUR:
      bitSet(items,5);
      element.state = STATE_HIDDEN;
      sound_item();
      break;

    case ITEM_BACKPACK:
      bitSet(items,6);
      element.state = STATE_HIDDEN;
      sound_item();
      break;

    case ITEM_SMELL:
      //Add smell....Yuck!
      bitSet(items,7);
      element.state = STATE_HIDDEN;
      sound_hit();

      if (life - 5 > 0)
      {
        life -= 5;
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }

      break;

    case ITEM_ROSE:
      element.state = STATE_HIDDEN;
      sound_hit();
      if (life - 5 > 0)
      {
        life -= 5;
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }
      break;

    case ITEM_POISON:
      element.state = STATE_HIDDEN;
      sound_hit();
      if (life - 10 > 0)
      {
        life -= 10;
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }
      break;

    case ITEM_SKULL:
      element.state = STATE_HIDDEN;
      sound_hit();
      if (life - 15 > 0)
      {
        life -= 15;
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }

      break;       

    case ITEM_SKULL2:
      element.state = STATE_HIDDEN;
      sound_hit();
      if (life - 20 > 0)
      {
        life -= 20;
      } 
      else {
        game_state = 14;//STATE_GAME_OVER
      }
      break;       

    case ITEM_FRUIT:
      element.state = STATE_HIDDEN;
      sound_item();
      if (life + 5 < 255) life +=5;
      break;         

    case ITEM_MEAT:
      element.state = STATE_HIDDEN;
      sound_item();
      if (life + 10 < 255) life +=10;
      break;        

    case ITEM_WINE:
      element.state = STATE_HIDDEN;
      sound_item();
      if (life + 15 < 255) life +=15;
      break;        

    case ITEM_HEALING:
      element.state = STATE_HIDDEN;
      sound_item();
      if (life + 20 < 255) life +=20;
      break;        

    case ITEM_MONEY:
      element.state = STATE_HIDDEN;
      sound_item();
      score +=2;
      //double score with backpack!
      if (bitRead(items,6) == 1) score +=2;
      break;        

    case ITEM_RING:
      element.state = STATE_HIDDEN;
      sound_item();
      score +=4;
      //double score with backpack!
      if (bitRead(items,6) == 1) score +=4;
      break;         

    case ITEM_DIAMOND:
      element.state = STATE_HIDDEN;
      sound_item();
      score +=8;
      //double score with backpack!
      if (bitRead(items,6) == 1) score +=8;       
      break;         

    case ITEM_CHEST:
      element.state = STATE_HIDDEN;
      sound_item();
      score +=10;
      //double score with backpack!
      if (bitRead(items,6) == 1) score +=10;       
      break;         
    }    
  }

  return element;
}




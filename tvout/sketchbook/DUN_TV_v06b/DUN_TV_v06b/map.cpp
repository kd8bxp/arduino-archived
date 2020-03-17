#include <avr/pgmspace.h>
#include <string.h>
#include "gamby_emulation.h"
#include "map.h"
#include "hero.h"
#include "item.h"
#include "enemy.h"
#include "map_blocks.h"
#include "map_layouts.h"
#include "map_tilesets.h"

//represent the current view coordinates
short view_x;
short view_y;

short map_num; //defines the map number
short tileset; //determines the tileset for the map

extern GambyEmulation gamby;
extern char level;

MapElement mapElements[MAX_ELEMENT_RECORDS];
char element_count = 0;


void map_init(short number)
{
  map_num = number;
  
  view_x = 0;
  view_y = 0;
  
  unsigned char rnd, i;
  
  //clear the array of the previous contents
  memset(mapElements, 0, sizeof(mapElements));
  element_count = 0;
  
  //randomly choose tileset
  tileset = (random() % 5);
    
  //add in the item (weapon) map element automatically, appropriate to the
  //type of hero selected
  map_element_add(ITEM_WEAPON + (get_hero_type() * 4), 8, 8, STATE_HIDDEN);
  
  //add exit
  map_element_place(ITEM_EXIT, STATE_HIDDEN);
  
  //add the key
  map_element_place(ITEM_KEY, STATE_VISIBLE);
  
  //add the map
  map_element_place(ITEM_MAP, STATE_VISIBLE);
 
  //add the compas
  map_element_place(ITEM_COMPASS, STATE_VISIBLE);

  //randomly place 3 items
  for (i=0; i<3; i++)
  {
     rnd = (random() % 16) + ITEM_INVISIBLE;
     map_element_place(rnd, STATE_VISIBLE);
  }
  
  //add random monsters
  unsigned char base = 0;
  //slowly increase level of monsters
  if (level > 25) base++;
  if (level > 50) base++;
  if (level > 75) base++;
  if (level > 100) base++;
  if (level > 125) base++;
  for (i=0; i< 12; i++)
  {
    rnd = (random() % 2) + base;
    map_element_place(rnd, STATE_VISIBLE);
  }
  
  hero_next_level();
  
}

void map_element_place(char type, char state)
{
  bool isPlaced = false;
  short placeX;
  short placeY;
  while (!isPlaced)
  {
    placeX = ((random() % 30)+1) * 8;
    placeY = ((random() % 30)+1) * 8;
    //check to see if it would be placed on a wall
    if (map_check_move(placeX, placeY) == 0)
    {
      //check to see if it would be on top of anything else
      //already placed
      
      //setting this to true so it can be tested
      isPlaced = true;
      for (char i=0; i < element_count; i++)
      {
        if (map_test_element(mapElements[i], placeX, placeY))
        {
          isPlaced = false;
          break;
        }
      }
      
    }
  }
  map_element_add(type, placeX, placeY, state);
}

void map_next_level()
{
  map_num++;
  map_init(map_num);
}

void map_element_add(char type, short x, short y, char state)
{
      mapElements[element_count].id = element_count;
      mapElements[element_count].type = type;
      mapElements[element_count].x = x;
      mapElements[element_count].y = y;
      mapElements[element_count].step = 1;
      mapElements[element_count].state = state;
      mapElements[element_count].step_dir = 0;
		  
      element_count++;  
}

void map_element_handle()
{
  for (char i=0; i < element_count; i++)
  {
    //if the element is hidden, skip past it
    if (mapElements[i].state > STATE_HIDDEN)
    {
      //to do: test for collision with hero
	  if (map_test_element(mapElements[i], get_hero_x(), get_hero_y()))
          {
            mapElements[i] = hero_hit(mapElements[i]);
          }
      
      if (mapElements[i].type <= 48)
      {	
         if (mapElements[0].state > STATE_HIDDEN)
         {
             if (map_test_element(mapElements[i], mapElements[0].x, mapElements[0].y)) 
             {
               mapElements[i] = enemy_hit(mapElements[i]);
               mapElements[0] = item_hit(mapElements[0]);
             }
         }    
	 mapElements[i] = enemy_move(mapElements[i]);
      } else {
	 mapElements[i] = item_move(mapElements[i]);
      }
    }
  }
}

char map_check_move(short x, short y)
{
  short map_x = x / 8;
  short map_y = y / 8;
  if ((map_x > 31) || (map_y > 31)) return 0;
  char test = map_get_block(map_x, map_y);
  if (test < 16) return 1;
  return 0;
}

//reads a specific map block (for mini-map)
char map_read_block(char block_x, char block_y, short number)
{
  //if the passed value is =1, use the current map number as 
  //the default
  if (number == -1) number = map_num;
  char block = pgm_read_byte_near ( map_layout + (16 * number) + (block_x * 4) + block_y);
  return block;
}

char map_get_block(char map_x, char map_y)
{
 //return pgm_read_byte_near(scroll_map + ((map_y * 32) + map_x));
 char layout_x = map_x / 8;
 char layout_y = map_y / 8;
 
 //find the block that the map coordinate would be in
 char block = pgm_read_byte_near ( map_layout + (16 * map_num) + (layout_y * 4) + layout_x);
 short block_idx = block * 64;
 
 //find where in the block that we are trying to access
 char block_x = map_x & 7;
 char block_y = map_y & 7;
 return pgm_read_byte_near (map_blocks + block_idx + ((block_y * 8) + block_x));
}

void map_move_view(char direction)
{  
   
   switch (direction)
   {
      case DIRECTION_UP:
                if (view_y > 0) view_y -= HERO_STEP_LENGTH;
		break;
		
      case DIRECTION_DOWN:
                if (view_y < 192) view_y += HERO_STEP_LENGTH;
		break;
	
      case DIRECTION_LEFT:
                if (view_x > 0 ) view_x -= HERO_STEP_LENGTH;
		break;	
		
      case DIRECTION_RIGHT:
                if (view_x < 160) view_x += HERO_STEP_LENGTH;
		break;			
   } 
}

short map_view_x()
{
  return view_x;
}

short map_view_y()
{
  return view_y;
}

char map_view_test(short x, short y)
{
  if ((x > view_x) && (x < (view_x + 96)))
  {
    if ((y > view_y) && (y < (view_y + 64))) return 1;
  }
  return 0;
}

bool map_test_elements(char testID, short testX, short testY)
{
  for (char i=0; i < element_count; i++)
  {
    //if the element is hidden, skip past it, and make sure not to test itself
    if ((mapElements[i].state > STATE_HIDDEN) && (mapElements[i].id != testID))
    {
	   if (map_test_element(mapElements[i], testX, testY)) return true;
	}
  }
  //last test -- see if it is not a block
  return map_check_move(testX, testY);
}

//test for collision in room elements
bool map_test_element (MapElement element, short testX, short testY)
{
   if ((element.y + 8) < testY) return false;
   if (element.y > (testY + 8)) return false;
   if ((element.x + 8) < testX) return false;
   if (element.x > (testX + 8)) return false;
   return true;
}

void map_draw()
{
  short mapx, mapy;
  short map_xoff, map_yoff;
  byte tile;
  mapx = map_view_x() / 8;
  mapy = map_view_y() / 8;
  
  map_xoff = map_view_x() & 7;
  map_yoff = map_view_y() & 7;
  
  for (int i=0; i < 9; i++)
  {
    for (int j=0; j < 13; j++)
    {
      tile = map_get_block((j+mapx), (i+mapy));
      if (tile > 0) gamby.drawSprite(j*8 - map_xoff, i*8 - map_yoff, map_tileset ,((tileset * TILESET_OFFSET) + tile)-1);
    }
  }
}

MapElement map_get_element(char num)
{
  return mapElements[num];
}

MapElement map_get_element_type(char type)
{
  MapElement findElement;
  findElement.type = 255;
  for (char i=0; i < element_count; i++)
  {
    if (mapElements[i].type == type)
    {
      findElement = mapElements[i];
      break;
    }
  }
  return findElement;
}

void map_update_element(MapElement element, char num)
{
  mapElements[num] = element;
}

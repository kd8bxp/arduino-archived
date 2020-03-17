#ifndef __MAP__
#define __MAP__

//define the step length of the hero
#define HERO_STEP_LENGTH 2

//define hero directions
#define DIRECTION_UP    0
#define DIRECTION_DOWN  1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT 3

#define MAX_ELEMENT_RECORDS 20

//Item definitions
#define ITEM_MAP           49
#define ITEM_COMPASS       50
#define ITEM_KEY           51
#define ITEM_EXIT          52
#define ITEM_INVISIBLE     53
#define ITEM_ARMOUR        54
#define ITEM_BACKPACK      55
#define ITEM_SMELL         56
#define ITEM_ROSE          57
#define ITEM_POISON        58
#define ITEM_SKULL         59
#define ITEM_SKULL2        60
#define ITEM_FRUIT         61
#define ITEM_MEAT          62
#define ITEM_WINE          63
#define ITEM_HEALING       64
#define ITEM_MONEY         65
#define ITEM_RING          66
#define ITEM_DIAMOND       67
#define ITEM_CHEST         68


//starting point for the item data
#define ITEM_WEAPON        69

//specific items
#define ITEM_WEAPON_ARROW  69
#define ITEM_WEAPON_AXE    73
#define ITEM_WEAPON_FIRE   77
#define ITEM_WEAPON_SWORD  81

//weapon facing constants
#define WEAPON_FACING_RIGHT 0
#define WEAPON_FACING_DOWN 1
#define WEAPON_FACING_LEFT 2
#define WEAPON_FACING_UP 3

//MapElement states
#define STATE_HIDDEN     0
#define STATE_VISIBLE    1
#define STATE_MOVE_LEFT  8
#define STATE_MOVE_RIGHT 2
#define STATE_MOVE_UP    11
#define STATE_MOVE_DOWN  5

#define TILESET_OFFSET   16

struct MapElement
{
  unsigned char id;          //elment identifier
  unsigned char type;        //element type
  unsigned short x;           // y position
  unsigned short y;           // x position
  unsigned char state;       //determines the state of the the element
  unsigned char step;        //used to maintain animation step
  unsigned char step_dir;    //step direction (up/down) - animation  
};

void map_init(short number);
void map_draw();
void map_element_add(char type, short x, short y, char state);
void map_element_handle();
char map_check_move(short x, short y);
char map_read_block(char block_x, char block_y, short number);
char map_get_block(char map_x, char map_y);
void map_move_view(char direction);
short map_view_x();
short map_view_y();
char map_view_test(short x, short y);
bool map_test_element (MapElement element, short testX, short testY);
bool map_test_elements(char testID, short testX, short testY);
MapElement map_get_element(char num);
void map_update_element(MapElement element, char num);
void map_next_level();
MapElement map_get_element_type(char type);
void map_element_place(char type, char state);
#endif __MAP__

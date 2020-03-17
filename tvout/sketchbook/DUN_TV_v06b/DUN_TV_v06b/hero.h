#include "map.h"

#define HERO_FACING_RIGHT 0
#define HERO_FACING_DOWN 3
#define HERO_FACING_LEFT 6
#define HERO_FACING_UP 9

#define HERO_STEP_LENGTH 2

#define HERO_STEP_UP 0
#define HERO_STEP_DOWN 1

#define HERO_OFFSET 12

#define HERO_TYPE_ELF 0
#define HERO_TYPE_WARRIOR 1
#define HERO_TYPE_WIZARD 2
#define HERO_TYPE_VALKYRIE 3

void hero_init(char type);
void hero_move(char facing);
void hero_use_weapon();
void hero_draw();
void hero_draw_pic(char x, char y);
short get_hero_x();
short get_hero_y();
char get_hero_type();
MapElement hero_hit(MapElement element);
void hero_next_level();

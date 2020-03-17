#ifndef __ENEMY__
#define __ENEMY__

#define ENEMY_STEP_UP 0
#define ENEMY_STEP_DOWN 1

//Monster definitions
#define ENEMY_OFFSET     12

#define ENEMY_TYPE_MINION   0
#define ENEMY_TYPE_BIGHEAD  1
#define ENEMY_TYPE_GHOST    2
#define ENEMY_TYPE_SKELETON 3
#define ENEMY_TYPE_DEATH    4
#define ENEMY_TYPE_DEVIL    5
#define ENEMY_TYPE_DRAGON   6




MapElement enemy_move(MapElement element);
MapElement enemy_hit(MapElement element);

#endif __ENEMY__

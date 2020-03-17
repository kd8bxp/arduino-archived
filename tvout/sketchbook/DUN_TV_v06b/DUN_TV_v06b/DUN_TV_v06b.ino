/*
 Dungeons: http://http://www.team-arg.org/DUN-manual.html
 
 TVOut version 0.6b : http://www.team-arg.org/DUN-downloads.html
 
 MADE by TEAM a.r.g. : http://www.team-arg.org/About.html
 
 2014 - Trodoss - JO3RI
 
 Stripped down version: only 20 levels and no QR-code !!!!
 
 */
 
//determine the game
#define GAME_ID 10
#include <TVout.h>
#include "gamby_emulation.h"
#include <avr/pgmspace.h>
#include <EEPROM.h>

#include "other_bitmap.h"
#include "text.h"
#include "font.h"

#include "map.h"
#include "hero.h"
#include "item.h"
#include "mini_map_tileset.h"

//determines the state of the game
unsigned char game_state = 0;
unsigned long next_action = 0;

//generic variable to store selection (on screens)
unsigned char selection = 0;
boolean yesno = true;

// These are all getting a value in STATE_GAME_INIT
unsigned char life;         // the character has 20 points of life when the game starts (max 255)
unsigned char experience;   // the character has a 1 points of experience when the game starts (max 255)
unsigned int score;         // the character has a 0 points of score when the game starts
unsigned char level;        // represents the current dungeon level


// there are 8 items that can be found. Not all items in all levels, but always: map, compass, key and exit
byte items = 0b00000000;        // this byte holds all the items the player collects during the game
//             ||||||||
//             |||||||└->  map          if found, you'll see all the borders of the walls on the map
//             ||||||└-->  compass      if found, you'll see the key or the exit on the map
//             |||||└--->  key          if found, you'll see the exit on the map
//             ||||└---->  exit         if found, you'll go to the next level. Only shown, when key is found
//             |||└----->  flask        if found, you are invisible for an amount of time, or until exit is found
//             ||└------>  shield       if found, you are invincible for an amount of time, or until exit is found
//             |└------->  backpack     if found, your gold (points) counts as double for an amount of time, or until exit is found
//             └-------->  smell        if found, enemies run away for an amount of time, or until exit is found



//? are these needed?
boolean pulseShow = false;

GambyEmulation gamby;
TVout TV;

#define PAUSE_BETWEEN_ACTIONS 80
#define PAUSE_BETWEEN_MENUS   160

//define menu states (on main menu)
#define STATE_MENU_MAIN    0
#define STATE_MENU_SELECT  1
#define STATE_MENU_HELP    2
#define STATE_MENU_CREDITS 3
#define STATE_MENU_NO_SAVE 4
#define STATE_MENU_LOAD    5


//define game states (when playing)
#define STATE_GAME_INIT         10
#define STATE_GAME_PLAYING      11
#define STATE_GAME_MAP_ITEMS    12
#define STATE_GAME_SAVE         13
#define STATE_GAME_OVER         14
#define STATE_GAME_END          15
#define STATE_GAME_NEXT_LEVEL   16

#define SHOW_LIFE               17
#define SHOW_EXPERIENCE         18
#define SHOW_SCORE              19
#define SHOW_LEVEL              20



//startup - show logo
void setup ()
{
  TV.begin(_NTSC, SCREEN_WIDTH, SCREEN_HEIGHT);  
  gamby.init();  
  gamby.font = font;
  gamby.drawPattern = PATTERN_BLACK;
  gamby.rect(0,0,95,63);
  gamby.drawSprite(2,16,TEAM_arg_logo);
  gamby.update();
  delay(3000L);

  game_state = STATE_MENU_MAIN;
}


//main loop
void loop()
{
  byte lastInputs;
  switch (game_state)
  {
    
//******* MENU-specific events **************
    
  case STATE_MENU_MAIN:
    menu_main();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             

      if (gamby.inputs & DPAD_LEFT)
      {
        if (selection > 0) selection--;
      }

      if (gamby.inputs & DPAD_RIGHT)
      {
        if (selection < 3) selection++;
      }
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        gamby.clearScreen();
        gamby.update();

        switch (selection)
        {
        case 0:
          game_state = STATE_MENU_HELP;
          menu_help();
          break;

        case 1:
          game_state = STATE_MENU_CREDITS;
          menu_credits();
          break;

        case 2:
          game_state = STATE_MENU_LOAD;
          break;

        case 3:
          game_state = STATE_MENU_SELECT;
          break;
        }
      }

      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;

  case STATE_MENU_SELECT:
    menu_select();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             

      if (gamby.inputs & DPAD_LEFT)
      {
        if (selection > 0) selection--;
      }

      if (gamby.inputs & DPAD_RIGHT)
      {
        if (selection < 3) selection++;
      }
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        game_state = STATE_GAME_INIT;
      }

      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;

  case STATE_MENU_HELP:
  case STATE_MENU_CREDITS:
  case STATE_MENU_NO_SAVE:
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             

      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        gamby.clearScreen();
        gamby.update();
        game_state = STATE_MENU_MAIN;
      }
      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;

  case STATE_MENU_LOAD:
    menu_load();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             
      if (gamby.inputs & DPAD_LEFT) {
        if (yesno < 1) yesno++;
      }
      if (gamby.inputs & DPAD_RIGHT) {
        if (yesno > 0) yesno--;
      }
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        switch (yesno){
        case true:
          switch (EEPROM.read(1023) == GAME_ID){
          case true:
            game_init(EEPROM.read(0));
            level = EEPROM.read(1);
            life = EEPROM.read(2);
            experience = EEPROM.read(3);
            score = 0;
            score = EEPROM.read(4);
            score = (score << 8) + EEPROM.read(5);
            game_state = STATE_GAME_NEXT_LEVEL;
            gamby.clearScreen();
            gamby.update();
            break;
          case false:
            gamby.setPos (0,6);
            gamby.print_P(noSavedgame);
            gamby.update();
            game_state = STATE_MENU_NO_SAVE;
            break;
          }
          break;
        case false:
          game_state = STATE_MENU_MAIN;    
          break;
        }
      }
      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;

//******* Game-specific events **************

  case STATE_GAME_INIT:
    score = 0;
    level = 1;
    life = 20;
    experience = 1;
    game_init(selection);
    break;

  case STATE_GAME_PLAYING:
    if (millis() >= next_action)
    {
      gamby.clearScreen();
      map_draw();
      map_element_handle();
      gamby.readInputs();
      lastInputs = gamby.inputs;	

      if (lastInputs & DPAD_UP) hero_move(HERO_FACING_UP);
      if (lastInputs & DPAD_DOWN) hero_move(HERO_FACING_DOWN);
      if (lastInputs & DPAD_RIGHT) hero_move(HERO_FACING_RIGHT);
      if (lastInputs & DPAD_LEFT) hero_move(HERO_FACING_LEFT);
      if (lastInputs & (BUTTON_2 | BUTTON_4)) hero_use_weapon();	 
      hero_draw();
      gamby.update();

      if (lastInputs & (BUTTON_1  | BUTTON_3 ))
      {
        game_state = STATE_GAME_MAP_ITEMS;
        clearScreen();
        }

      if (life < 1)
      {
        clearScreen();
        game_state = STATE_GAME_OVER;
      }

      if (level > 250)
      {
        clearScreen();
        game_state = STATE_GAME_END;
      }

      next_action = millis() + PAUSE_BETWEEN_ACTIONS; 	 
    }
    break;


  case STATE_GAME_MAP_ITEMS:
    game_paused(); 
    if (millis() >= next_action)
    {
      gamby.readInputs();
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        game_state = STATE_GAME_PLAYING;
        }
      pulseShow = !pulseShow;
      next_action = millis() + PAUSE_BETWEEN_MENUS;
    }  
    break;

  case STATE_GAME_SAVE:
    if (level >= 250){
      game_state = STATE_GAME_END;
      break;
    }
    game_save();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      if (gamby.inputs & DPAD_LEFT) if (yesno < 1) yesno++;
      if (gamby.inputs & DPAD_RIGHT) if (yesno > 0) yesno--;
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        switch (yesno)
        {
        case true:
          //EEPROM.write save game
          EEPROM.write(0, get_hero_type());
          EEPROM.write(1, level+1);
          EEPROM.write(2, life);
          EEPROM.write(3, experience);
          EEPROM.write(4, score >> 8);
          EEPROM.write(5, score);
          EEPROM.write(1023, GAME_ID);
        }
        //start the next level          
        map_next_level();
        game_state = STATE_GAME_NEXT_LEVEL;
        level++;
        gamby.clearScreen();
        gamby.update();
      }
      next_action = millis() + PAUSE_BETWEEN_MENUS;
    }
    break;

  case STATE_GAME_OVER:
    clearScreen();
    game_over();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        game_state = STATE_MENU_MAIN;
      }
      next_action = millis() + PAUSE_BETWEEN_ACTIONS;
    }
    break;

  case STATE_GAME_END:
    clearScreen();
    game_end();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        game_state = STATE_MENU_MAIN;
      }
      next_action = millis() + PAUSE_BETWEEN_MENUS;
    }
    break;

  case STATE_GAME_NEXT_LEVEL:
    game_next_level();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        gamby.clearScreen();
        game_state = STATE_GAME_PLAYING;
      }
      next_action = millis() + PAUSE_BETWEEN_ACTIONS;
    }
    break;
  }
}

void clearScreen()
{  
  gamby.drawPattern = PATTERN_WHITE;
  gamby.rect(0,0,95,63);
  gamby.drawPattern = PATTERN_BLACK;
}


//****************** Menu-specific functions
void menu_main()
{  
  // clean up the screen
  clearScreen();

  // show the splash art
  gamby.drawSprite(40,27,ungeons_splash);
  gamby.drawSprite(16,0,Dungeons_gate);
  gamby.drawSprite(53,-1,closed_door);
  gamby.drawSprite(0,-2,big_D_splash);


  // text menu choice
  gamby.setPos(4,7);
  gamby.print_P(textmenu);
  gamby.drawSprite(13+(22*selection)-2,47,selector);
  gamby.update();  
}

void menu_select()
{
  clearScreen();

  // show the splash art
  gamby.drawSprite(0,0,Dungeons_gate);

  // Player character selection

  gamby.setPos(28,7);
  switch (selection){
    case 0:
      gamby.print_P(elf);
      break;
    case 1:
      gamby.print_P(warrior);
      break;
    case 2:
      gamby.print_P(wizard);
      break;
    case 3:
      gamby.print_P(valkyrie);
      break;
  }
    
  for (byte i = 0; i<4;i++)
  {
    gamby.drawSprite(20+(16*i),39,small_character_bitmap,i);
  } 
  gamby.drawSprite(20+(16*selection)-4,35,big_character_bitmap,selection);
  gamby.update();  
}


void menu_credits()
{
  clearScreen();
  gamby.drawSprite(13,0,small_D_splash);
  gamby.drawSprite(29,1,ungeons_splash);
  gamby.setPos (0,3);
  gamby.print_P(Credits);
  gamby.update();  
}

void menu_help()
{
  clearScreen();
  gamby.drawSprite(19,-4,QRcode);
  gamby.update();  
}

void menu_load()
{
  clearScreen();
  gamby.drawSprite(24,-1,disk);
  gamby.setPos (10,7);
  gamby.print_P(loadgameText);
  gamby.print_P(yesnoText);
  gamby.drawSprite(78-(15*yesno),47,selector); 
  gamby.update();
}

//****************** Game-specific functions
void game_init(char selected_type)
{
  hero_init(selected_type);
  game_start_level();
}

void game_paused()
{
  byte miniMapPositionHero_x = (get_hero_x()/4);
  byte miniMapPositionHero_y = (get_hero_y()/4);
  byte miniMapPositionKey_x = item_get_x(ITEM_KEY)/4;
  byte miniMapPositionKey_y = item_get_y(ITEM_KEY)/4;
  byte miniMapPositionExit_x = item_get_x(ITEM_EXIT)/4;
  byte miniMapPositionExit_y = item_get_y(ITEM_EXIT)/4;
  gamby.setPos(65,0);
  if (score<1000) gamby.print("0");
  add_zero_print(score,0);
  gamby.print("0");
  gamby.setPos(65,1);
  add_zero_print(level,0);
  gamby.setPos(65,5);
  add_zero_print(life,SHOW_LIFE);
  gamby.setPos(65,6);
  add_zero_print(experience,SHOW_EXPERIENCE);
  hero_draw_pic(65,54);

  // if the map is in the items-list, show mini-map
  if (bitRead(items,0) == 1)  // create the mini-map
  {
    for (short m = 0; m<4; m++){
      for (short k = 0; k<4; k++)
      {
        gamby.drawSprite((16*k),(m*16)-1,mini_map_tileset, map_read_block(m,k, -1));
      }
    }
    item_draw(64,24,0);  // show the map in the items list.
  }
  else  // show an empty mini-map
  {
    for (short m = 0; m<4; m++){
      for (short k = 0; k<4; k++)
      {
        gamby.drawSprite((16*k),(m*16)-1,mini_map_tileset, map_read_block(m,k, 0));
      }   
    }
  }

  // show the compass in the items list.
  if (bitRead(items,1) == 1)
  {
    item_draw(72,24,1);  // show the compas in the items list.

    // we need to show the players position on the map
    gamby.setPixel(miniMapPositionHero_x, miniMapPositionHero_y,pulseShow);
    gamby.setPixel(miniMapPositionHero_x+1, miniMapPositionHero_y,pulseShow);
    gamby.setPixel(miniMapPositionHero_x, miniMapPositionHero_y+1,pulseShow);
    gamby.setPixel(miniMapPositionHero_x+1, miniMapPositionHero_y+1,pulseShow); 

    // we show the players pulsating pixels in the list (in the bottom)
    gamby.setPixel(76, 59,pulseShow);
    gamby.setPixel(76, 60,pulseShow);
    gamby.setPixel(77, 59,pulseShow);
    gamby.setPixel(77, 60,pulseShow);

    // we show "key or exit" pulsating pixels in the list (in the bottom)
    gamby.setPixel(92, 59,0);
    gamby.setPixel(92, 60,!pulseShow);
    gamby.setPixel(93, 59,pulseShow);
    gamby.setPixel(93, 60,0); 

    if (bitRead(items,2) == 0)  // if key isn't found yet, we show it's position 
    {
      item_draw(83,55,2);  //we show a key icon on the bottom of the list
      // we show the key's position on the map
      gamby.setPixel(miniMapPositionKey_x, miniMapPositionKey_y,0);
      gamby.setPixel(miniMapPositionKey_x, miniMapPositionKey_y+1,!pulseShow);
      gamby.setPixel(miniMapPositionKey_x+1, miniMapPositionKey_y,pulseShow);
      gamby.setPixel(miniMapPositionKey_x+1, miniMapPositionKey_y+1,0);
    }
    else  //if the key has been found, we show the position of the exit
    {
      item_draw(83,55,3);  //we show an exit icon on the bottom of the list
      //show the position of the exit
      gamby.setPixel(miniMapPositionExit_x, miniMapPositionExit_y,0);
      gamby.setPixel(miniMapPositionExit_x, miniMapPositionExit_y+1,!pulseShow);
      gamby.setPixel(miniMapPositionExit_x+1, miniMapPositionExit_y,pulseShow);
      gamby.setPixel(miniMapPositionExit_x+1, miniMapPositionExit_y+1,0);
    }
  }
  if (bitRead(items,2) == 1) item_draw(80,24,2);  // show the key in the items list.
  for (byte i = 0; i<4; i++)
  {
    if (bitRead(items,4+i) == 1) item_draw(64+(i*8),31,4+i);
  }
  gamby.update();
}

void game_next_level()
{
  clearScreen();
  score_level(level);
  gamby.drawSprite(15,4,next_level);
  gamby.update();
}


void game_over(){
  gamby.drawSprite(14,12,bmp_game_over);
  gamby.setPos(8,4);
  add_zero_print(score,SHOW_SCORE);
  gamby.setPos(8,5);
  add_zero_print(level,SHOW_LEVEL);
  gamby.rect(0,55,95,64);
  gamby.rect(0,0,95,8);
  for (byte k=0; k<2;k++)
  {
    for (byte i= 0; i< 8;i++)
    {
      gamby.drawSprite(2+(i*12),55*k,skull);
    }
  }
  gamby.update();
}

void game_end(){
  gamby.setPos(0,1);
  gamby.print_P(gameEndText);
  gamby.setPos(0,7);
  add_zero_print(score,SHOW_SCORE);
  gamby.drawSprite(60,-1,crystal);
  gamby.update();
}

void game_save(){
  clearScreen();
  gamby.drawSprite(24,-1,disk);
  gamby.setPos (10,7);
  gamby.print_P(savegameText);
  gamby.print_P(yesnoText);
  gamby.drawPattern = PATTERN_WHITE;
  gamby.rect(0,48,95,55);
  gamby.drawPattern = PATTERN_BLACK;
  gamby.drawSprite(78-(15*yesno),47,selector);
  gamby.update();
}

void game_start_level()
{
  clearScreen();
  game_state = STATE_GAME_NEXT_LEVEL;
  gamby.update();  
  map_init(level);
}

void score_level(byte n)
{
  gamby.setPos(26,4);
  add_zero_print(n,SHOW_LEVEL);
  gamby.setPos(51,5);
  add_zero_print(life,SHOW_LIFE);
  gamby.setPos(50,6);
  add_zero_print(experience,SHOW_EXPERIENCE);
  gamby.setPos(21,7);
  add_zero_print(score,SHOW_SCORE);
  gamby.drawSprite(21,37,big_character_bitmap, get_hero_type());
}

void add_zero_print(unsigned int n, unsigned char s)
{
  switch (s){
    case SHOW_LIFE:
      gamby.print_P(lifeText);
      break;
    case SHOW_EXPERIENCE:
      gamby.print_P(experienceText);
      break;
    case SHOW_LEVEL:
      gamby.print_P(levelText);
      break;
    case SHOW_SCORE:
      gamby.print_P(scoreText);
      if (n<1000) gamby.print("0");
      break;
  }
  if (n<100) gamby.print("0");
  if (n<10) gamby.print("0");
  gamby.print(n);
  if (s == SHOW_SCORE)gamby.print("0");
}








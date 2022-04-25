/**
 * @brief It defines a textual graphic engine
 *
 * @file graphic_engine.c
 * @author Miguel Soto, Antonio Van-Oers, Nicolas Victorino and Ignacio Nunez
 * @version 3.0
 * @date 06-04-2022
 * @copyright GNU Public License
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/graphic_engine.h"
#include "../include/libscreen.h"
#include "../include/command.h"
#include "../include/space.h"
#include "../include/types.h"
#include "../include/enemy.h"
#include "../include/player.h"
#include "../include/object.h"
#include "../include/inventory.h"
#include "../include/set.h"


#define ROWS 39       /*!< Establish screen size in y dimension */
#define COLUMNS 120   /*!< Establish screen size in x dimension */

/**
 * @brief Graphic engine
 *
 * This struct contains all pointer to the different areas shown in graphics
 */
struct _Graphic_engine
{
  Area *map;      /*!< graphic engine area of the game map */
  Area *descript; /*!< graphic engine area of the description */
  Area *banner;   /*!< graphic engine area of the banner */
  Area *help;     /*!< graphic engine area of help */
  Area *feedback;   /*!< graphic engine area of feedback */
};

/**
 * graphic_engine_create initializes game's members
 */
Graphic_engine *graphic_engine_create()
{
  static Graphic_engine *ge = NULL;

  if (ge)
    return ge;

  screen_init(ROWS, COLUMNS);
  ge = (Graphic_engine *)malloc(sizeof(Graphic_engine));
  if (ge == NULL)
    return NULL;

  ge->map = screen_area_init(1, 1, 80, 25);
  ge->descript = screen_area_init(82, 1, 35, 25);
  ge->banner = screen_area_init(53, 29, 14, 1);
  ge->help = screen_area_init(1, 30, 115, 3);
  ge->feedback = screen_area_init(1, 34, 115, 3);

  return ge;
}

/**
  * graphic_engine_destroy, using the function screen_area_destroy, 
  * destroys the map, banner, descript, help and feedback 
  * It also frees all the previosly alloc'd memory
  */
void graphic_engine_destroy(Graphic_engine *ge)
{
  if (!ge)
    return;

  screen_area_destroy(ge->map);
  screen_area_destroy(ge->descript);
  screen_area_destroy(ge->banner);
  screen_area_destroy(ge->help);
  screen_area_destroy(ge->feedback);

  screen_destroy();
  free(ge);
}

/**
  * Prints the game on the screen, with all its components on the 
  * right position for the user to see the info needed to interact with it
  */
void graphic_engine_paint_game(Graphic_engine *ge, Game *game, int st)
{
  /* Variables declaration */
  Id id_act = NO_ID, id_up = NO_ID, id_down = NO_ID, id_left= NO_ID, id_right= NO_ID;
  Id obj_loc[MAX_OBJS] = {NO_ID}, player_loc = NO_ID, en_loc[MAX_ENEMYS] = {NO_ID}, obj_id[MAX_OBJS]= {NO_ID};
  Inventory *player_inventory = NULL;
  int en_health[MAX_ENEMYS] = {0}, player_health = 0;
  Space *space_act = NULL;
  char obj = '\0', obj_l = '\0', obj_r = '\0';
  char str[255];
  T_Command last_cmd = UNKNOWN;
  extern char *cmd_to_str[N_CMD][N_CMDT];
  int i;
  char **gdesc = NULL, **gdesc_right = NULL, **gdesc_left = NULL;
  char *description;
  char *inspection;
  char *obj_name[MAX_OBJS];
  char link_up = '\0', link_down = '\0';

  /* setting all proper values for each variable */
  player_loc = game_get_player_location(game, 21);
  id_act = player_loc;
  player_inventory = player_get_inventory(game_get_player(game, game_get_player_id(game)));
  player_health = player_get_health(game_get_player(game, game_get_player_id(game)));
  description = (char*) space_get_brief_description(game_get_space(game, player_loc));
  inspection = game_get_inspection(game);

  for(i=0;i<MAX_ENEMYS;i++)
  { 
    /* Error Control */
    if (game_get_enemy(game, game_get_enemy_id(game, i))==NULL)
    {
      break;
    }
    en_health[i] = enemy_get_health(game_get_enemy(game, game_get_enemy_id(game, i)));
    en_loc[i] = game_get_enemy_location(game, game_get_enemy_id(game, i));
  }

  for(i=0;i<MAX_OBJS;i++)
  { 
    /* Error Control */
    if (game_get_object(game, game_get_object_id(game, i))==NULL)
    {
      break;
    }
    obj_id[i] = game_get_object_id(game, i);
    obj_loc[i] = obj_get_location(game_get_object(game, game_get_object_id(game, i)));
    obj_name[i] = (char *) obj_get_name(game_get_object(game, game_get_object_id(game, i)));
  }

  /* Paint the in the map area */
  screen_area_clear(ge->map);
  
  if (id_act != NO_ID)
  {
    space_act = game_get_space(game, id_act);
    id_up = space_get_id_dest_by_link(game_get_link(game, space_get_link(space_act, N)));
    id_down = space_get_id_dest_by_link(game_get_link(game, space_get_link(space_act, S)));
    id_left = space_get_id_dest_by_link(game_get_link(game, space_get_link(space_act, W)));
    id_right = space_get_id_dest_by_link(game_get_link(game, space_get_link(space_act, E)));

    space_print(space_act);

    /* Space to the north of the current space */
    if (id_up != NO_ID)
    {
      for(i=0;i<MAX_OBJS;i++)
      { 
        /* Checks whether there is an object or not in that space */
        if (space_has_object(game_get_space(game, id_up), obj_id[i]) == FALSE)
        {         
          obj = ' ';
        }
        else
        {
          obj = '*';
          break;
        }
      }

      if (id_up < 100)
      {
        sprintf(str, "  |              %2d |", (int)id_up);
      }
      else
      {
        sprintf(str, "  |              %2d|", (int)id_up);
      }
      screen_area_puts(ge->map, str);

      sprintf(str, "  |                 |");
      screen_area_puts(ge->map, str);
      sprintf(str, "  |        %c        |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |                 |");
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------------+");
      screen_area_puts(ge->map, str);
      
      if (game_get_connection_status(game, id_act, N) == OPEN)
      {
        link_up = '^';
      }
      else
      {
        link_up = ' ';
      }

      sprintf(str, "          %c", link_up);
      screen_area_puts(ge->map, str);
    }

    /* Current space */
  
    if (id_act != NO_ID)
    {
      
      for(i=0;i<MAX_OBJS;i++)
      {   
        /* Checks whether there is an object or not in that space */    
        if (space_has_object(game_get_space(game, id_act), obj_id[i]) == FALSE)
        {         
          obj = ' ';
        }
        else
        {
          obj = '*';
          break;
        }
      }

       /* Checks that there are no other spaces to the left or right */    
      if(id_left==NO_ID && id_right==NO_ID)  
      {
        sprintf(str, "  +-----------------+");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |");
        screen_area_puts(ge->map, str);
        /*\\(\")/ ANT CODIFICATION */
        if (id_act < 100)
        {
          sprintf(str, "  | gpp0^        %2d |", (int)id_act);
        }
        else
        {
          sprintf(str, "  | gpp0^        %2d|", (int)id_act);
        }
        screen_area_puts(ge->map, str);

        sprintf(str, "  |                 |");
        screen_area_puts(ge->map, str);
        gdesc = space_get_gdesc(game_get_space(game, id_act));
        
        for (i = 0; i < TAM_GDESC_Y; i++)
        {
          sprintf(str, "  |    %s    |", gdesc[i]);
          screen_area_puts(ge->map, str);
        }
        
        sprintf(str, "  |                 |");
        screen_area_puts(ge->map, str);
        
        sprintf(str, "  |        %c        |", obj);
        screen_area_puts(ge->map, str);

        sprintf(str, "  |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  +-----------------+");
        screen_area_puts(ge->map, str);
      } 
      
      /* Checks that there is no space to the left but there is one to the right */    
      if (id_right != NO_ID && id_left == NO_ID)
      {  

      for(i=0;i<MAX_OBJS;i++)
      { 
        /* Checks whether there is an object or not in that space */
        if (space_has_object(game_get_space(game, id_right), obj_id[i]) == FALSE)
        {         
          obj_r = ' ';
        }
        else
        {
          obj_r = '*';
          break;
        }
      }

        sprintf(str, "  +------------------+   ------------------|");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                  |   |                 |");
        screen_area_puts(ge->map, str);
        /*\\(\")/ ANT CODIFICATION */
        sprintf(str, "  | gpp0^         %2ld|   |              %2ld|", id_act, id_right);
        screen_area_puts(ge->map, str);
        sprintf(str, "  |       %c          |   |       %c         |", obj, obj_r);
        screen_area_puts(ge->map, str);
        gdesc = space_get_gdesc(game_get_space(game, id_act)); 
        gdesc_right = space_get_gdesc(game_get_space(game, id_right)); 
        
        for (i = 0; i < TAM_GDESC_Y; i++)
        {
          if (i != 3) {
            sprintf(str, "  |     %s    |   |    %s    |", gdesc[i], gdesc_right[i]);
            screen_area_puts(ge->map, str);
          } else {
            sprintf(str, "  |     %s    | > |    %s    |", gdesc[i], gdesc_right[i]);
            screen_area_puts(ge->map, str);
          }
        }
        
        sprintf(str, "  |                  |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                  |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                  |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  +------------------+>> ------------------|");
        screen_area_puts(ge->map, str);  
      }
      
      /* Checks that there are both spaces to the left and right */    
      if (id_left != NO_ID && id_right != NO_ID)
      {
        for (i = 0; i < 4; i++) 
        {
        sprintf(str, " ");
        screen_area_puts(ge->map, str);
        }

      for(i=0;i<MAX_OBJS;i++)
      { 
        /* Checks whether there is an object or not in that space */
        if (space_has_object(game_get_space(game, id_right), obj_id[i]) == FALSE)
        {         
          obj_r = ' ';
        }
        else
        {
          obj_r = '*';
          break;
        }
      }

      for(i=0;i<MAX_OBJS;i++)
      { 
        /* Checks whether there is an object or not in that space */
        if (space_has_object(game_get_space(game, id_left), obj_id[i]) == FALSE)
        {         
          obj_l = ' ';
        }
        else
        {
          obj_l = '*';
          break;
        }
      }
        
        sprintf(str, "  |-----------------    +-----------------+   ------------------|");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |   |                 |");
        screen_area_puts(ge->map, str);
        /*\\(\")/ ANT CODIFICATION */
        if (id_right != 14) {
          sprintf(str, "  |              %2ld|   | gpp0^        %2ld|   |              %2ld|",id_left, id_act, id_right);
          screen_area_puts(ge->map, str);
        } else {
          sprintf(str, "  |              %2ld|   | gpp0^        %2ld|   |              %2ld |",id_left, id_act, id_right);
          screen_area_puts(ge->map, str);
        }
        sprintf(str, "  |         %c       |   |         %c       |   |         %c       |", obj_l, obj, obj_r);
        screen_area_puts(ge->map, str);
        gdesc = space_get_gdesc(game_get_space(game, id_act)); 
        gdesc_right = space_get_gdesc(game_get_space(game, id_right)); 
        gdesc_left = space_get_gdesc(game_get_space(game, id_left)); 
        
        for (i = 0; i < TAM_GDESC_Y; i++)
        {
          if (i != 3) {
            sprintf(str, "  |    %s    |   |    %s    |   |    %s    |", gdesc_left[i], gdesc[i], gdesc_right[i]);
            screen_area_puts(ge->map, str);
          } else {
            sprintf(str, "  |    %s    | < |    %s    | > |    %s    |", gdesc_left[i], gdesc[i], gdesc_right[i]);
            screen_area_puts(ge->map, str);
          }
        }
        
        sprintf(str, "  |                 |   |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |   |                 |");
        screen_area_puts(ge->map, str);  
        sprintf(str, "  |-----------------    +-----------------+   ------------------|");
        screen_area_puts(ge->map, str);
      }

      /* Checks that there is no space to the right but there is one to the left */    
      if (id_right == NO_ID && id_left != NO_ID && id_act==14)
      {  
        for (i = 0; i < 4; i++) 
        {
        sprintf(str, " ");
        screen_area_puts(ge->map, str);
        }

      for(i=0;i<MAX_OBJS;i++)
      { 
        /* Checks whether there is an object or not in that space */
        if (space_has_object(game_get_space(game, id_left), obj_id[i]) == FALSE)
        {         
          obj_l = ' ';
        }
        else
        {
          obj_l = '*';
          break;
        }
      }

        sprintf(str, "  |------------------   +-----------------+");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |");
        screen_area_puts(ge->map, str);
        /*\\(\")/ ANT CODIFICATION */
        sprintf(str, "  |              %2ld|   |  gpp0^       %2ld |", id_left, id_act);
        screen_area_puts(ge->map, str);
        sprintf(str, "  |        %c        |   |        %c        |", obj_l, obj);
        screen_area_puts(ge->map, str);
        gdesc = space_get_gdesc(game_get_space(game, id_act)); 
        gdesc_left = space_get_gdesc(game_get_space(game, id_left)); 
        
        for (i = 0; i < TAM_GDESC_Y; i++)
        {
          if (i != 3) {
            sprintf(str, "  |     %s   |   |     %s   |", gdesc_left[i], gdesc[i]);
            screen_area_puts(ge->map, str);
          }else {
            sprintf(str, "  |     %s   | > |     %s   |", gdesc_left[i], gdesc[i]);
            screen_area_puts(ge->map, str);
          }
        }
        
        sprintf(str, "  |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |                 |   |                 |");
        screen_area_puts(ge->map, str);
        sprintf(str, "  |-----------------    <<+---------------+");
        screen_area_puts(ge->map, str);  
      }
          
    }

    /* Space to the south of the current space */
    if (id_down != NO_ID && id_act != 14)
    {
      for(i=0;i<MAX_OBJS;i++)
      {   
        /* Checks whether there is an object or not in that space */    
        if (space_has_object(game_get_space(game, id_down), obj_id[i]) == FALSE)
        {         
          obj = ' ';
        }
        else
        {
          obj = '*';
          break;
        }
      }

      if (game_get_connection_status(game, id_act, S) == OPEN)
      {
        link_down = 'v';
      }
      else
      {
        link_down = ' ';
      }

      sprintf(str, "           %c", link_down);
      screen_area_puts(ge->map, str);
      sprintf(str, "  +-----------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "  |                 |");
      screen_area_puts(ge->map, str);
      if (id_down != 13) {
        sprintf(str, "  |              %2d|", (int)id_down);
        screen_area_puts(ge->map, str);
      } else {
        sprintf(str, "  |              %2d |", (int)id_down);
        screen_area_puts(ge->map, str);   
      }
      sprintf(str, "  |        %c        |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "  |                 |");
      screen_area_puts(ge->map, str);
    }
    
  }

      /* Space to the south of the current space CASE 14 */
    if (id_down != NO_ID && id_act == 14)
    {
      for(i=0;i<MAX_OBJS;i++)
      {   
        /* Checks whether there is an object or not in that space */    
        if (space_has_object(game_get_space(game, id_down), obj_id[i]) == FALSE)
        {         
          obj = ' ';
        }
        else
        {
          obj = '*';
          break;
        }
      }

      sprintf(str, "                                  v");
      screen_area_puts(ge->map, str);
      sprintf(str, "                        +-----------------+");
      screen_area_puts(ge->map, str);
      sprintf(str, "                        |                 |");
      screen_area_puts(ge->map, str);
      sprintf(str, "                        |              %2d |", (int)id_down);
      screen_area_puts(ge->map, str);
      sprintf(str, "                        |        %c        |", obj);
      screen_area_puts(ge->map, str);
      sprintf(str, "                        |                 |");
      screen_area_puts(ge->map, str);
    }

  /* Paint in the description area */
  screen_area_clear(ge->descript);
  
  if ((player_loc != NO_ID) && (en_loc[0] != NO_ID))
  {
    sprintf(str, "  Objects location:");
    screen_area_puts(ge->descript, str);
    
    for(i=0;i<MAX_OBJS;i++)
    { 
      if (game_get_object(game, game_get_object_id(game, i))==NULL)
      {
        break;                     
      }
     sprintf(str, "  %s:%d, ", obj_name[i], (int)obj_loc[i]);
     screen_area_puts(ge->descript, str);
    }

    sprintf(str, "   ") ;
    screen_area_puts(ge->descript, str);
    sprintf(str, "  Player location:%d", (int)player_loc);
    screen_area_puts(ge->descript, str);
  
    if(player_inventory==NULL || set_get_nids(inventory_get_objects(player_inventory)) == 0)
    {
      sprintf(str, "  Player object: No object" );
      screen_area_puts(ge->descript, str);
    }
    else
    {
      sprintf(str, "  Player objects:"); 
      screen_area_puts(ge->descript, str);

    
      for (i = 0; i < set_get_nids(inventory_get_objects(player_inventory)); i++)
      {
        sprintf(str, "    %s", obj_get_name(game_get_object(game, set_get_ids_by_number(inventory_get_objects(player_inventory),i)))); 
        screen_area_puts(ge->descript, str);
      }
    }
    
    sprintf(str, "  Player health:%d", (int)player_health);
    screen_area_puts(ge->descript, str);

    
    sprintf(str, "   ") ;
    screen_area_puts(ge->descript, str);
    sprintf(str, "  Enemy location:%d", (int)en_loc[0]);
    screen_area_puts(ge->descript, str);
    sprintf(str, "  Enemy health:%d", (int)en_health[0]);
    screen_area_puts(ge->descript, str);

    sprintf(str, "   ");
    screen_area_puts(ge->descript, str);
    sprintf(str, "  Info:");
    screen_area_puts(ge->descript, str);
    sprintf(str, "  %s", description);
    screen_area_puts(ge->descript, str);

    sprintf(str, "   ");
    screen_area_puts(ge->descript, str);
    sprintf(str, "  Descriptions:");
    screen_area_puts(ge->descript, str);
    if(inspection){
      sprintf(str, "  %s", inspection);
      screen_area_puts(ge->descript, str);
    }
  }

  /* Paint in the banner area */
  screen_area_puts(ge->banner, " The Ant-Game "); 

  /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, " w(UP), s(DOWN), a(LEFT), d(RIGHT), c(TAKE), v(DROP), q(ATTACK), i(INSPECT), m(MOVE), e(EXIT)");
  screen_area_puts(ge->help, str);
  /*
  sprintf(str, " Player object will be -1 as long as it doesn't carry one");
  screen_area_puts(ge->help, str);
  */

  /* Paint in the feedback area */
  last_cmd = game_get_last_command(game);
  if (st == 0)
  {
    sprintf(str, " %s (%s): ERROR", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  }
  else if (st == 1)
  {
    sprintf(str, " %s (%s): OK", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  }
  else
  {
    sprintf(str, " %s (%s)", cmd_to_str[last_cmd - NO_CMD][CMDL], cmd_to_str[last_cmd - NO_CMD][CMDS]);
  }
  
  screen_area_puts(ge->feedback, str);

  /* Dump to the terminal */
  screen_paint();
  printf("prompt:> ");
}


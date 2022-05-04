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

#define ROWS 44       /*!< Establish screen size in y dimension */
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
  Area *dialogue; /*!< graphic engine area of dialoge */
  Area *events; /*!< graphic engine area of events */
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
  ge->banner = screen_area_init(50, 29, 19, 1);
  ge->help = screen_area_init(1, 30, 115, 3);
  ge->feedback = screen_area_init(1, 34, 80, 3);
  ge->dialogue = screen_area_init(1, 38, 80, 5);
  ge->events = screen_area_init(82, 34, 35, 9);

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
  screen_area_destroy(ge->dialogue);
  screen_area_destroy(ge->events);

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
  Id id_act = NO_ID, id_up = NO_ID, id_down = NO_ID, id_left= NO_ID, id_right= NO_ID, aux_obj_id = NO_ID;
  Id player_loc = NO_ID, en_loc[MAX_ENEMYS] = {NO_ID};
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
  char *en_name[MAX_ENEMYS];
  char *inspection, *dialogue, *events;
  char *obj_name[MAX_OBJS];
  char link_up = '\0', link_down = '\0', link_right = '\0', link_left = '\0';
  Set *object_set = NULL;

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
    en_name[i] = (char *)enemy_get_name(game_get_enemy(game, game_get_enemy_id(game, i)));
    en_loc[i] = game_get_enemy_location(game, game_get_enemy_id(game, i));
  }

  for(i=0;i<MAX_OBJS;i++)
  { 
    /* Error Control */
    if (game_get_object(game, game_get_object_id(game, i))==NULL)
    {
      break;
    }
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

    object_set = space_get_objects(game_get_space(game, id_act));
    space_print(space_act);

    /* Space to the north of the current space */
    if (id_up != NO_ID)
    {
      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          i = set_get_nids(object_set) + 1;
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
      
      if (game_get_connection_status(game, id_act, N) == OPEN_L)
      {
        link_up = '^';
      }
      else
      {
        link_up = ' ';
      }

      if (game_get_connection_status(game, id_act, E) == OPEN_L)
      {
        link_right = '>';
      }
      else
      {
        link_right = ' ';
      }

      if (game_get_connection_status(game, id_act, W) == OPEN_L)
      {
        link_left = '<';
      }
      else
      {
        link_left = ' ';
      }

      sprintf(str, "          %c", link_up);
      screen_area_puts(ge->map, str);
    }

    /* Current space */
  
    if (id_act != NO_ID)
    {
      
      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          i = set_get_nids(object_set) + 1;
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

      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          i = set_get_nids(object_set) + 1;
        }
      }

      if (game_get_connection_status(game, id_act, E) == OPEN_L)
      {
        link_right = '>';
      }
      else
      {
        link_right = ' ';
      }

      if (game_get_connection_status(game, id_act, W) == OPEN_L)
      {
        link_left = '<';
      }
      else
      {
        link_left = ' ';
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
            sprintf(str, "  |     %s    | %c |    %s    |", gdesc[i], link_right, gdesc_right[i]);
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

      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          i = set_get_nids(object_set) + 1;
        }
      }

      if (game_get_connection_status(game, id_act, E) == OPEN_L)
      {
        link_right = '>';
      }
      else
      {
        link_right = ' ';
      }

      if (game_get_connection_status(game, id_act, W) == OPEN_L)
      {
        link_left = '<';
      }
      else
      {
        link_left = ' ';
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
            sprintf(str, "  |    %s    | %c |    %s    | %c |    %s    |", gdesc_left[i], link_left, gdesc[i], link_right, gdesc_right[i]);
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

      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          break;
        }
      }
  
      if (game_get_connection_status(game, id_act, E) == OPEN_L)
      {
        link_right = '>';
      }
      else
      {
        link_right = ' ';
      }

      if (game_get_connection_status(game, id_act, W) == OPEN_L)
      {
        link_left = '<';
      }
      else
      {
        link_left = ' ';
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
            sprintf(str, "  |     %s   | %c |     %s   |", gdesc_left[i], link_left, gdesc[i]);
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
      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
          obj = '*';              
          break;
        }
      }

      if (game_get_connection_status(game, id_act, S) == OPEN_L)
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
      for(i = 0; i < set_get_nids(object_set); i++)
      {
        aux_obj_id = set_get_ids_by_number(object_set, i);

        if (obj_is_visible(game_get_object(game, aux_obj_id), space_get_light_status(game_get_space(game, id_act))) ==  FALSE) 
        {
          obj = ' ';
        } else {
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
    sprintf(str, "  Objects in space:");
    screen_area_puts(ge->descript, str);
    
    for(i=0;i< set_get_nids(space_get_objects(game_get_space(game, id_act)));i++)
    { 
      if (game_get_object(game,set_get_ids_by_number(space_get_objects(game_get_space(game, id_act)), i)) == NULL)
      {
        break;                     
      }

      if (obj_is_visible(game_get_object(game,set_get_ids_by_number(space_get_objects(game_get_space(game, id_act)), i)), space_get_light_status(game_get_space(game, id_act)))== TRUE) {
        sprintf(str, "  %s ", obj_name[i]);
        screen_area_puts(ge->descript, str);
      }
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

    sprintf(str, "  Enemies in space:") ;
    screen_area_puts(ge->descript, str);
    for (i = 0; i < MAX_ENEMYS; i++) {
      if (en_loc[i] == id_act) {
        sprintf(str, "   ") ;
        screen_area_puts(ge->descript, str);
        /*
        sprintf(str, "  Enemy location:%d", (int)en_loc[i]);
        screen_area_puts(ge->descript, str);
        */
        sprintf(str, "  Enemy name:%s", en_name[i]);
        screen_area_puts(ge->descript, str);
        sprintf(str, "  Enemy health:%d", (int)en_health[i]);
        screen_area_puts(ge->descript, str);
        sprintf(str, "   ") ;
        screen_area_puts(ge->descript, str);
      }
    }

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
  screen_area_puts(ge->banner, " Day-Night Dungeon "); 

  /* Paint in the help area */
  screen_area_clear(ge->help);
  sprintf(str, " The commands you can use are:");
  screen_area_puts(ge->help, str);
  sprintf(str, " w(UP), s(DOWN), a(LEFT), d(RIGHT), c(TAKE), v(DROP), q(ATTACK), i(INSPECT), m(MOVE), e(EXIT), s (SAVE), l (LOAD),");
  screen_area_puts(ge->help, str);
  sprintf(str, " o (OPEN), ton (TURNON), toff (TURNOFF)");
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

/* prints the dialoge area */
screen_area_clear(ge->dialogue);

screen_area_puts(ge->dialogue, " Dialogue:");
dialogue = dialogue_get_command(game_get_dialogue(game));

if (dialogue)
{
  sprintf(str, "  %s", dialogue);
  screen_area_puts(ge->dialogue, str);
}

/* prints the events area */
screen_area_clear(ge->events);

screen_area_puts(ge->events, " Events:");
events = dialogue_get_event(game_get_dialogue(game));
if (events)
{
  sprintf(str, "  %s", events);
  screen_area_puts(ge->events, str);
}


  /* Dump to the terminal */
  screen_paint();
  printf("prompt:> ");

}

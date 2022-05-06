/**
 * @brief It implements the game module and all the associated calls
 * for each command
 *
 * @file game.c
 * @author Antonio Van-Oers, Miguel Soto, Nicolas Victorino, Ignacio Nunez
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../include/game.h"
#include "../include/game_managment.h"

/**
 * @brief Game
 * 
 * This struct stores all the information of a space.
 */
struct _Game
{
  Player *player[MAX_PLAYERS]; /*!< Pointer to player's array */
  Object *object[MAX_OBJS];    /*!< Pointer to object's array */
  Enemy *enemy[MAX_ENEMYS];    /*!< Pointer to enemy's array */
  Space *spaces[MAX_SPACES];   /*!< Pointer to space's array */
  Link *links[MAX_LINKS];      /*!< Pointer to link's array */
  char *inspection;            /*!< Long description for inspect space */
  Time day_time;               /*!< Time cycle of the game */
  T_Command last_cmd;          /*!< Last command input */
  R_Event last_event;          /*!< Last event */
  Dialogue *dialogue;          /*!< Text to print*/
} ;

/**
   Private functions
*/
int game_command_unknown(Game *game, char *arg);
void game_get_new_event(Game *game);
STATUS game_command_exit(Game *game, char *arg);
STATUS game_command_take(Game *game, char *arg);
STATUS game_command_drop(Game *game, char *arg);
STATUS game_command_attack(Game *game, char *arg);
STATUS game_command_move(Game *game, char *arg);
STATUS game_command_movement(Game *game, DIRECTION dir);
STATUS game_command_inspect(Game *game, char *arg);
STATUS game_command_save(Game *game, char *arg);
STATUS game_command_load(Game *game, char *arg);
STATUS game_command_turnon(Game *game, char *arg);
STATUS game_command_turnoff(Game *game, char *arg);
STATUS game_command_open(Game *game, char *link_name, char *obj_name);
STATUS game_command_use(Game *game, char *arg);
STATUS game_event_move(Game *game);
STATUS game_event_trap(Game *game);
STATUS game_event_slime(Game *game);
STATUS game_event_daynight(Game *game);
STATUS game_event_spawn(Game *game);

/**
 * Game interface implementation
 */

/** game_create allocs memory for some necessary game's members
 */
STATUS game_alloc(Game *game)
{
  Id id_player = 21, id_enemy = 41;
  int i = 0;

  if (!game)
  {
    fprintf(stderr, "Error saving memory for game (game_alloc)");
    return ERROR;
  }

  for (i = 0; i < MAX_PLAYERS; i++)
  {
    game->player[i] = player_create(id_player);
    if (!game->player[i])
    {
      return ERROR;
    }
  }

  for (i = 0; i < MAX_ENEMYS; i++)
  {
    game->enemy[i] = enemy_create(id_enemy);
    if (!game->enemy[i])
    {
      return ERROR;
    }
  }

  game->inspection = " ";
  game->dialogue = dialogue_create();
  return OK;
}

/** game_create initializes all necessary game's members
 * to a empty value
 */
STATUS game_create(Game *game)
{
  int i = 0;

  /* Error control*/
  if (game == NULL)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES; i++)
  {
    game->spaces[i] = NULL;
  }

  for (i = 0; i < MAX_OBJS; i++)
  {
    game->object[i] = NULL;
  }

  for (i = 0; i < MAX_PLAYERS; i++)
  {
    game->player[i] = NULL;
  }

  for (i = 0; i < MAX_ENEMYS; i++)
  {
    game->enemy[i] = NULL;
  }

  for (i = 0; i < MAX_LINKS; i++)
  {
    game->links[i] = NULL;
  }

  game->day_time = DAY;
  game->last_cmd = NO_CMD;
  
  return OK;
}

/** game_destroy frees/destroys all of game's members
 * calling space_destroy for each one of them
 */
STATUS game_destroy(Game *game)
{
  int i = 0;

  /* Error control*/
  if (game == NULL)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
    obj_destroy(game->object[i]);
    game->object[i] = NULL;
  }

  for (i = 0; i < MAX_PLAYERS && game->player[i] != NULL; i++)
  {
    player_destroy(game->player[i]);
    game->player[i] = NULL;
  }

  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    enemy_destroy(game->enemy[i]);
    game->enemy[i] = NULL;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_destroy(game->spaces[i]);
    game->spaces[i] = NULL;
  }

  for (i = 0; i < MAX_LINKS; i++)
  {
    link_destroy(game->links[i]);
    game->links[i] = NULL;
  }

  free(game);
  game = NULL;

  return OK;
}

/**
 * It adds one space (space) to game's struct.
 */
STATUS game_add_space(Game *game, Space *space)
{
  int i = 0;

  /* Error control*/
  if (game == NULL)
  {
    return ERROR;
  }

  /* Error control*/
  if (space == NULL)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
  }

  /* Error control*/
  if (i >= MAX_SPACES)
  {
    return ERROR;
  }

  game->spaces[i] = space;

  return OK;
}

/**
 * Adds objects to the game
 */
STATUS game_add_object(Game *game, Object *obj)
{
  int i = 0;

  /* Error control*/
  if (!game || !obj)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
  }

  /* Error control*/
  if (i >= MAX_OBJS)
  {
    return ERROR;
  }

  game->object[i] = obj;

  return OK;
}

/**
 * It adds a player to game struct.
 */
STATUS game_add_player(Game *game, Player *p)
{
  int i = 0;

  /* Error control*/
  if (!game || !p)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_PLAYERS && game->player[i] != NULL; i++)
  {
  }

  if (i >= MAX_PLAYERS)
  /* Error control*/
  {
    return ERROR;
  }

  game->player[i] = p;

  return OK;
}

/**
 * Adds all game's enemies
 */
STATUS game_add_enemy(Game *game, Enemy *e)
{
    int i = 0;

  /* Error control*/
  if (!game || !e)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
  }

  /* Error control*/
  if (i >= MAX_ENEMYS)
  {
    return ERROR;
  }

  game->enemy[i] = e;

  return OK;
}

/**
 * Adds all game's links
 */
STATUS game_add_link(Game *game, Link *l)
{
  int i = 0;

  /* Error control*/
  if (!game || !l)
  {
    return ERROR;
  }

  for (i = 0; i < MAX_LINKS && game->links[i] != NULL; i++)
  {
  }

  /* Error control*/
  if (i >= MAX_LINKS)
  {
    return ERROR;
  }

  game->links[i] = l;

  return OK;
}

/**
 * game_get_space_id_at Checks if the spaces are within the appropiate range, output will be No_ID
 * whenever it is located outside this range or its id if it belongs to the game
 */
Id game_get_space_id_at(Game *game, int position)
{
  /* Error control*/
  if (position < 0 || position >= MAX_SPACES)
  {
    return NO_ID;
  }

  return space_get_id(game->spaces[position]);
}

/** Gets game's space with target id
 */
Space *game_get_space(Game *game, Id id)
{
  int i;

  /* Error control*/
  if (!game || id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    if (id == space_get_id(game->spaces[i]))
    {
      return game->spaces[i];
    }
  }

  return NULL;
}

/** Sets the day time of the game
 */
STATUS game_set_time(Game *game, Time time)
{
  if (!game || (time != DAY && time != NIGHT))
  {
    return ERROR;
  }

  game->day_time = time;
  return OK;
}

/** Gets if it is day or night
 */
Time game_get_time(Game *game)
{
  if (!game)
  {
    return -1;
  }

  return game->day_time;
}

/**
 * Gets a game's object with target id
 */
Object *game_get_object(Game *game, Id id)
{
  int i;

  /* Error control*/
  if (!game || id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
    if (id == obj_get_id(game->object[i]))
    {
      return game->object[i];
    }
  }

  return NULL;
}

/**
 * @brief Finds an object with the same name. Case is ignore.
 * @author Nicolas Victorino
 * 
 * @param game pointer to game struct
 * @param name string with obj_name
 * @return pointer to object with name name.
 */
Object *game_get_object_byName(Game *game, char *name)
{
  int i;

  /* Error control*/
  if (!game || name == NULL)
  {
    return NULL;
  }

  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
    if (strcasecmp(name, obj_get_name(game->object[i])) == 0)
    {
      return game->object[i];
    }
  }

  return NULL;
}

/**
 * @brief Finds a link with the same name. Case is ignore.
 * @author Miguel Soto
 * 
 * @param game pointer to game struct.
 * @param name string with link's name.
 * @return pointer to link with name name.
 */
Link *game_get_link_byName(Game *game, char *name)
{
  int i;

  /* Error control*/
  if (!game || name == NULL)
  {
    return NULL;
  }

  for (i = 0; i < MAX_LINKS && game->links[i] != NULL; i++)
  {
    if (strcasecmp(name, link_get_name(game->links[i])) == 0)
    {
      return game->links[i];
    }
  }

  return NULL;
}

Enemy *game_get_enemyWithPlayer(Game *game, Id player_loc){

  int i;

  if (!game || player_loc == NO_ID){
    return NULL;
  }

  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    if (player_loc == enemy_get_location(game->enemy[i]))
    {
      return game->enemy[i];
    }
  }

  return NULL;

}

/**
 * Finds an enemy with the same name. Case is ignore.
 */
Enemy *game_get_enemy_byName(Game *game, char *name)
{
  int i;

  /* Error control*/
  if (!game || name == NULL)
  {
    return NULL;
  }

  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    if (strcasecmp(name, enemy_get_name(game->enemy[i])) == 0)
    {
      return game->enemy[i];
    }
  }

  return NULL;
}

/**
 * Gets a game's palyer with target id
 */
Player *game_get_player(Game *game, Id id)
{
  int i;

  /* Error control*/
  if (!game || id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; i < MAX_PLAYERS && game->player[i] != NULL; i++)
  {
    if (id == player_get_id(game->player[i]))
    {
      return game->player[i];
    }
  }

  return NULL;
}

/**
 * Gets a game's enemy with target id
 */
Enemy *game_get_enemy(Game *game, Id id)
{
  int i;

  /* Error control*/
  if (!game || id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    if (id == enemy_get_id(game->enemy[i]))
    {
      return game->enemy[i];
    }
  }

  return NULL;
}

/**
 * returns long description that is on screen
 */
char *game_get_inspection(Game *game)
{
  /* Error control*/
  if (!game)
  {
    return NULL;
  }

  return game->inspection;
}

/**
 * Gets a game's link with target id
 */
Link *game_get_link(Game *game, Id id)
{
  int i;

  /* Error control*/
  if (!game || id == NO_ID)
  {
    return NULL;
  }

  for (i = 0; i < MAX_LINKS && game->links[i] != NULL; i++)
  {
    if (id == link_get_id(game->links[i]))
    {
      return game->links[i];
    }
  }

  return NULL;
}

/**
 * Sets the player's location to target id
 */
STATUS game_set_player_location(Game *game, Id player_id, Id space_id)
{
  Player *p = NULL;

  /* Error control*/
  if (!game || player_id == NO_ID || space_id == NO_ID)
  {
    return ERROR;
  }

  p = game_get_player(game, player_id);
  if (!p)
  {
    return ERROR;
  }

  /* Error control*/
  if (player_set_location(p, space_id) == ERROR)
  {
    return ERROR;
  }

  return OK;
}

/**
 * Sets the object's location to target id
 */
STATUS game_set_object_location(Game *game, Id obj_id, Id space_id)
{
  Object *o = NULL;

  /* Error control*/
  if (!game || obj_id == NO_ID)
  {
    return ERROR;
  }

  o = game_get_object(game, obj_id);
  /* Error control*/
  if (!o)
  {
    return ERROR;
  }

  /* Error control*/
  if (obj_set_location(o, space_id) == ERROR)
  {
    return ERROR;
  }

  return OK;
}

/**
 * Sets the enemy's location to target id
 */
STATUS game_set_enemy_location(Game *game, Id enemy_id, Id space_id)
{
    Enemy *e = NULL;

  /* Error control*/
  if (!game || enemy_id == NO_ID || space_id == NO_ID)
  {
    return ERROR;
  }

  e = game_get_enemy(game, enemy_id);
  /* Error control*/
  if (!e)
  {
    return ERROR;
  }

  return enemy_set_location(game->enemy[MAX_PLAYERS - 1], space_id);
}

/** Gets an player's position
 */
Id game_get_player_location(Game *game, Id player_id)
{
  Player *p = NULL;

  /* Error control*/
  if (!game || player_id == NO_ID)
  {
    return NO_ID;
  }

  p = game_get_player(game, player_id);
  /* Error control*/
  if (!p)
  {
    return NO_ID;
  }

  return player_get_location(p);
}

/** Gets an enemy's position
 */
Id game_get_enemy_location(Game *game, Id enemy_id)
{
  Enemy *e = NULL;

  /* Error control*/
  if (!game || enemy_id == NO_ID)
  {
    return NO_ID;
  }

  e = game_get_enemy(game, enemy_id);
  /* Error control*/
  if (!e)
  {
    return NO_ID;
  }

  return enemy_get_location(e);
}

/** Gets an object's position
 */
Id game_get_object_location(Game *game, Id obj_id)
{
    Object *o = NULL;

  /* Error control*/
  if (!game || obj_id == NO_ID)
  {
    return NO_ID;
  }

  o = game_get_object(game, obj_id);
  /* Error control*/
  if (!o)
  {
    return NO_ID;
  }

  return obj_get_location(o);
}

/**
 * It get if link is open or close giving an space and a specific direction
 */
LINK_STATUS game_get_connection_status(Game *game, Id act_spaceid, DIRECTION dir)
{
  Id link_id = -1;
  Space *s = NULL;
  LINK_STATUS linkst = CLOSE;

  /* Error control */
  if (!game || act_spaceid == NO_ID || dir == ND)
  {
    return CLOSE;
  }

  s = game_get_space(game, act_spaceid);
  /* Error control*/
  if (!s)
  {
    return CLOSE;
  }

  link_id = space_get_link(s, dir);
  /* Error control*/
  if (link_id < 0)
  {
    return CLOSE;
  }

  linkst = link_get_status(game_get_link(game, link_id));

  return linkst;
}

/**
 * It get id from the space destination of a link
 */
Id game_get_connection(Game *game, Id act_spaceid, DIRECTION dir)
{
  Id link_id = -1, dest_id = -1;
  Space *s = NULL;

  /* Error control */
  if (!game || act_spaceid == NO_ID || dir == ND)
  {
    return CLOSE;
  }

  s = game_get_space(game, act_spaceid);

  /* Error control */
  if (!s)
  {
    return CLOSE;
  }

  link_id = space_get_link(s, dir);
  /* Error control */
  if (link_id < 0)
  {
    return CLOSE;
  }

  dest_id = link_get_destination(game_get_link(game, link_id));

  return dest_id;
}

/*Functions for the new event
(just a continuation of game_update)*/
void game_get_new_event(Game *game){

  R_Event new_event;
  STATUS st;

  new_event = game_rules_get_event();
  game->last_event = new_event;

  switch(new_event)
  {
    case MOVE_OBJ:
      st = game_event_move(game);
      break;

    case TRAP:
      st = game_event_trap(game);
      break;

    case SLIME:
      st = game_event_slime(game);
      break;

    case DAYNIGHT:
      st = game_event_daynight(game);
      break;

    case SPAWN:
      st = game_event_spawn(game);
      break;

    default:
      break;
  }

  if(st == ERROR){
    game->last_event = NOTHING;
  }

  return;
}


/** game_update updates cmd whenever something is typed in, this is where
 * every known case gets interpreted as the various actions on the game,
 * any other input would be considered unknown
 */
int game_update(Game *game, T_Command cmd, char *arg1, char *arg2)
{
  int st = 0;
  
  if(!game){
    return 0;
  }
  
  game->last_cmd = cmd;
  game->inspection = 0;
  dialogue_reset(game->dialogue);

  switch (cmd)
  {
  case UNKNOWN:
    st = game_command_unknown(game, arg1);
    break;

  case EXIT:
    st = (int)game_command_exit(game, arg1);
    break;

  case TAKE:
    st = (int)game_command_take(game, arg1);
    break;

  case DROP:
    st = (int)game_command_drop(game, arg1);
    break;

  case ATTACK:
    st = (int)game_command_attack(game, arg1);
    break;

  case MOVE:
    st = (int)game_command_move(game, arg1);
    break;

  case INSPECT:
    st = (int)game_command_inspect(game, arg1);
    break;

  case SAVE:
    st = (int)game_command_save(game, arg1);
    break;

  case LOAD:
    st = (int)game_command_load(game, arg1);
    break;
    
  case TURNON:
    st = (int)game_command_turnon(game, arg1);
    break;

  case TURNOFF:
    st = (int)game_command_turnoff(game, arg1);
    break;

  case OPEN:
    st = (int)game_command_open(game, arg1, arg2);
    break;

  case USE:
    st = (int)game_command_use(game, arg1);
    break;
      
  default:
    break;
  }
  
  game_get_new_event(game);

  return st;
}


/* Gets the last event in the input
*/
R_Event game_get_last_event(Game *game){
  return game->last_event;
}

/** Gets the last command in the input
 */
T_Command game_get_last_command(Game *game)
{
  return game->last_cmd;
}

/**
 * game_print_data prints all game related relevant data
 */
void game_print_data(Game *game)
{
  int i = 0;

  printf("\n\n-------------\n\n");

  printf("=> Spaces: \n");
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_print(game->spaces[i]);
  }

  printf("=> Objects:\n");
  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
    obj_print(game->object[i]);
  }

  printf("=> Players:\n");
  for (i = 0; i < MAX_PLAYERS && game->player[i] != NULL; i++)
  {
    player_print(game->player[i]);
  }

  printf("=> Enemies:\n");
  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    enemy_print(game->enemy[i]);
  }
}

/** False is the output when called
 * meaning game has ended
 */
BOOL game_is_over(Game *game)
{
  if (player_get_health(game->player[MAX_PLAYERS - 1]) <= 0)
  {
    return TRUE;
  }

  return FALSE;
}

/**
 *Calls implementation for each action
 */

/**
 * @brief It executes UNKNOWN command in game.
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
int game_command_unknown(Game *game, char *arg)
{
  return 5;
}

/**
 * @brief It executes EXIT command in game.
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_exit(Game *game, char *arg)
{
  return OK;
}

/**
 * @brief It executes TAKE command in game.
 * 
 * This command is for the player to take the object located
 * on the current position. It requires the name of the object for it
 * to be taken correctly
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_take(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id id_obj_taken = NO_ID, obj_loc = NO_ID;
  Object *obj_taken;
  Space *s = NULL;
  Object *o = NULL;
  STATUS st = OK;

  obj_taken = game_get_object_byName(game, arg);
  id_obj_taken = obj_get_id(obj_taken);
  
  /* Error control*/
  if(object_get_movable(obj_taken)==TRUE && obj_is_visible(obj_taken, space_get_light_status(game_get_space(game, player_location)))){  
    /* Error control*/
    if (space_has_object(game_get_space(game, player_location), id_obj_taken) && set_get_nids(inventory_get_objects(player_get_inventory(game->player[0]))) < inventory_get_maxObjs(player_get_inventory(game->player[0])))
    {
      s = game_get_space(game, player_location);
      o = game_get_object(game, id_obj_taken);
      obj_loc = game_get_object_location(game, id_obj_taken);

      /* Error control*/
      if (player_location != obj_loc)
      {
        st = ERROR;
      }

      /* Error control*/
      if (s == NULL)
      {
        st = ERROR;
      }

      /* Error control*/
      if (o == NULL)
      {
        st = ERROR;
      }

      if (game_set_object_location(game, obj_get_id(o), NO_ID) == ERROR)
      /* Error control */
      {
        st = ERROR;
      }

      /* Error control */
      if (player_add_object(game->player[MAX_PLAYERS - 1], o) == ERROR)
      {
        st = ERROR;
      }
      /* Error control */
      if (space_del_objectid(s, id_obj_taken) == ERROR)
      {
        st = ERROR;
      }
     
      return st;
    }
  }

  st=ERROR;
  return st;
}

/**
 * @brief It executes DROP command in game.
 * 
 * This command is for dropping the object that carries the player
 * on the current position. It requires the name of the object to be dropped
 * to work correctly
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_drop(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id obj_id = NO_ID;
  Object *obj;
  STATUS st = OK;
  Space *s = NULL;
  Object *o = NULL;

  obj = game_get_object_byName(game, arg);
  obj_id = obj_get_id(obj);

  /* Error control */
  if (obj_id != obj_get_id(obj) || inventory_has_id(player_get_inventory(game->player[MAX_PLAYERS - 1]), obj_id) == FALSE)
  {
    st = ERROR;
  }

  /* Error control*/
  if (player_get_inventory(game->player[MAX_PLAYERS - 1]) == NULL)
  {
   st = ERROR;
  }

  s = game_get_space(game, player_location);
  /* Error control*/
  if (s == NULL)
  {
    st = ERROR;
  }

  o = game_get_object(game, obj_id);
  /* Error control*/
  if (o == NULL)
  {
    st = ERROR;
  }

  /* Error control */
  if (space_add_objectid(s, obj_get_id(o)) == ERROR)
  {
    st = ERROR;
  }

  /* Error control */
  if (game_set_object_location(game, obj_get_id(o), space_get_id(s)) == ERROR)
  {
    st = ERROR;
  }
  /* Error control */
  if (player_del_object(game->player[MAX_PLAYERS - 1], obj_id) == ERROR)
  {
    st = ERROR;
  }

  /* If the object is the ladder or Candle_1(Turnedon==TRUE and are dropped in the correct space, make them not movable) */
  if((obj_id==31 && space_get_id(s)==11) || (obj_id==315 && space_get_id(s)==13 && object_get_turnedon(obj)==TRUE)){
    object_set_movable(obj, FALSE);
  }
  return st;
}

/**
 * @brief It executes ATTACK command in game.
 * 
 * This command allows the player to engage in combat with an enemy.
 * The deciding factor is up to chance, a random number will determine
 * whether the player or the enemy won that round resulting in the loser
 * getting his health reduced by 1
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_attack(Game *game, char *arg)
{
  int rand_num = 0, rand_crit_player = 0, rand_crit_enemy = 0;
  Id player_loc = player_get_location(game->player[MAX_PLAYERS - 1]);
  Enemy *enemy = game_get_enemyWithPlayer(game, player_loc);
  Id enemy_loc = enemy_get_location(enemy);
  
  int player_crit = player_get_crit(game->player[MAX_PLAYERS - 1]);
  int player_baseDmg = player_get_baseDmg(game->player[MAX_PLAYERS - 1]);

  int enemy_crit = enemy_get_crit(enemy);
  int enemy_baseDmg = enemy_get_baseDmg(enemy);
 

  srand(time(NULL));

  rand_num = rand() % 10;
  rand_crit_player = rand() % 10;
  rand_crit_enemy = rand() % 10;

  /* Error control */
  if(!enemy)
  {
    return ERROR;
  }

  /* Error control */
  if (enemy_get_health(enemy) == 0)
  {
    return ERROR;
  }

  /* Error control */
  if (player_loc == NO_ID || enemy_loc == NO_ID)
  {
    return ERROR;
  }

  /* Error control */
  if (player_loc != enemy_loc)
  {
    return ERROR;
  }


  /* Changing the base dmg if it is a crit from the player*/
  if(rand_crit_player < player_crit){
    player_baseDmg = player_baseDmg*2;
  }
  if(rand_crit_enemy < enemy_crit){
    enemy_baseDmg = enemy_baseDmg*2;
  }


  /*Player wins if rand_num es > 5, else, they lose a life as the enemy won that round*/
  if (rand_num > 5)
  {
    enemy_set_health(enemy, (enemy_get_health(enemy) - player_baseDmg));
  }
  else
  {
    player_set_health(game->player[MAX_PLAYERS - 1], (player_get_health(game->player[MAX_PLAYERS - 1]) - enemy_baseDmg));
    if (player_get_health(game->player[MAX_PLAYERS - 1]) == 0)
    {
      game_is_over(game);
    }
  }

  return OK;
}

/**
 * @brief It executes MOVE command in game.
 * 
 * Another way to move, commands move n,s,e,w,u,d
 * to north, south, east, west, up or down respectively
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_move(Game *game, char *arg)
{
  char *west[2] = {"w", "West"};
  char *north[2] = {"n", "North"};
  char *south[2] = {"s", "South"};
  char *east[2] = {"e", "East"};
  char *up[2] = {"u", "Up"};
  char *down[2] = {"d", "Down"};
  STATUS st = ERROR;

  if (strcasecmp(arg, west[0]) == 0 || strcasecmp(arg, west[1]) == 0)
  {
    /* Moving west */
    st = game_command_movement(game, W);
  }
  else if (strcasecmp(arg, north[0]) == 0 || strcasecmp(arg, north[1]) == 0)
  {
    /* Moving north */
    st = game_command_movement(game, N);
  }
  else if (strcasecmp(arg, south[0]) == 0 || strcasecmp(arg, south[1]) == 0)
  {
    /* Moving south */
    st = game_command_movement(game, S);
  }
  else if (strcasecmp(arg, east[0]) == 0 || strcasecmp(arg, east[1]) == 0)
  {
    /* Moving east */
    st = game_command_movement(game, E);
  }
  else if (strcasecmp(arg, up[0]) == 0 || strcasecmp(arg, up[1]) == 0)
  {
    /* Moving up */
    st = game_command_movement(game, U);
  }
  else if (strcasecmp(arg, down[0]) == 0 || strcasecmp(arg, down[1]) == 0)
  {
    /* Moving down */
    st = game_command_movement(game, D);
  }
  else
  {
    st = ERROR;
  }

  return st;
}

/**
 * @brief It executes the movement from a move command in game.
 * @author Miguel Soto
 * 
 * @param game pointer to game struct
 * @param dir movement direction
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_movement(Game *game, DIRECTION dir)
{
  Id player_location = NO_ID, player_id = NO_ID;
  Space *s = NULL;
  Link *l = NULL;
  STATUS st = OK;
  if (!game || dir == ND)
  {
   st = ERROR;
  }

  player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  player_id = player_get_id(game->player[MAX_PLAYERS - 1]);
  if (player_location == NO_ID || player_id == NO_ID)
  {
    st = ERROR;
  }
  
  s = game_get_space(game, player_location);
  if (!s)
  {
    st = ERROR;
  }
  
  l = game_get_link(game, space_get_link(s, dir));
  if (!l)
  {
    st = ERROR;
  }
  
  if (link_get_status(l) == OPEN_L)
  {
    game_set_player_location(game, player_id, link_get_destination(l));
    return st;
  }
  else
  {
    /* Link is closed */
    st = ERROR;
    return st;
  }
}


/**
 * @brief It executes INSPECT command in game.
 * 
 * Changes the description of game to the one user wanted
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_inspect(Game *game, char *arg)
{
  Object *obj = game_get_object_byName(game, arg);
  STATUS st = OK;

  /*SPACE CASE*/
  if (strcmp(arg, "space") == 0 || strcmp(arg, "s") == 0)
  {
    if(space_get_light_status(game_get_space(game, player_get_location(game->player[MAX_PLAYERS - 1]))) == BRIGHT){
      game->inspection = (char *)space_get_long_description(game_get_space(game, player_get_location(game->player[MAX_PLAYERS - 1])));
    }
    else{
      game->inspection = "El lugar está muy oscuro, no puedes ver nada";
    }
    return st;
  }

  /*OBJECT CASE*/
  else
  {
    if (arg == NULL)
    {
      game->inspection = " ";
      st = ERROR;
    }

    if (obj == NULL)
    {
      game->inspection = " ";
      st = ERROR;
    }
    if (player_has_object(game->player[0], obj_get_id(obj)) == FALSE && player_get_location(game->player[0]) != obj_get_location(obj))
    {
      st = ERROR;
    }
    game->inspection = (char *)obj_get_description(obj);
    return st;
  }
  return st;
}

/**
 * @brief It executes SAVE command in game.
 * 
 * Creates a file with the info of the game
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_save(Game* game, char *arg){
  return game_managment_save(arg, game);
}

/**
 * @brief It executes LOAD command in game.
 * 
 * Generates a new game throught a file 
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_load(Game* game, char *arg){
  STATUS st = OK;
  if(strcasecmp(arg, "savedata.dat") != 0 && strcasecmp(arg, "hormiguero.dat") != 0){
    st = ERROR;
  }

 st = game_managment_load(arg, game);
 return st;
}

/**
 * @brief It executes TURNON command in game
 * @author Miguel Soto
 *
 * It turn on an object so it iluminates.
 *
 * @param game pointer to game struct.
 * @param arg string with command argument.
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_turnon(Game *game, char *arg)
{
  Object *obj = NULL;

  printf("**\nEntrando en turnon\n");
  if (!game || !arg)
  {
    return ERROR;
  }

  printf("Getting obj\n");
  obj = game_get_object_byName(game, arg);
  if (!obj)
  {
    printf("ERROR Obj is NULL\n**\n");
    return ERROR;
  }

  if (player_has_object(game->player[MAX_PLAYERS-1], obj_get_id(obj)) == FALSE)
  {
    return ERROR;
  }
  printf("Checking obj properties\n");
  if (object_get_illuminate(obj) == FALSE || object_get_turnedon(obj) == TRUE)
  {
    /* Object has not iluminate attribute or object is already iluminated*/
    printf("ERROR object properties\n**\n");
    return ERROR;
  }

  printf("Calling object_set_turnon");
  return object_set_turnedon(obj, TRUE);
}

/**
 * @brief It executes TURN OFF command in game
 * @author Miguel Soto
 *
 * It turn off an object so it stop to iluminate.
 *
 * @param game pointer to game struct.
 * @param arg string with command argument.
 * @return OK if everything goes well or ERROR if there was any mistake .
 */
STATUS game_command_turnoff(Game *game, char *arg)
{
  Object *obj = NULL;

  printf("**\nEntrando en turnon\n");
  if (!game || !arg)
  {
    return ERROR;
  }

  printf("Getting obj\n");
  obj = game_get_object_byName(game, arg);
  if (!obj)
  {
    printf("ERROR Obj es NULL\n**\n");
    return ERROR;
  }

  if (player_has_object(game->player[MAX_PLAYERS-1], obj_get_id(obj)) == FALSE)
  {
    return ERROR;
  }
  printf("Checking obj properties\n");
  if (object_get_illuminate(obj) == FALSE || object_get_turnedon(obj) == FALSE)
  {
    /* Object has not iluminate attribute or object is already not iluminated*/
    printf("ERROR object properties\n**\n");
    return ERROR;
  }

  printf("Calling object_set_turnon FALSE object properties\n**\n");
  return object_set_turnedon(obj, FALSE);
}

/**
 * @brief It executes OPEN command in game
 * @author Miguel Soto
 * 
 * It opens a link with an object.
 * 
 * @param game pointer to game struct.
 * @param link_name string with link name that is about to be open.
 * @param obj_name string with object name that opens link.
 * @return OK if everything goes well or ERROR if there was any mistake .
 */
STATUS game_command_open(Game *game, char *link_name, char *obj_name)
{
  Link *l = NULL;
  Object *obj = NULL;
  Id obj_open_link = NO_ID;

  if (!game || !link_name || !obj_name)
  {
    return ERROR;
  }
  
  l = game_get_link_byName(game, link_name);
  obj = game_get_object_byName(game, obj_name);
  if (!l || !obj)
  {
    return ERROR;
  }
  
  if (link_get_status(l) == OPEN_L)
  {
    /* Link is already open */
    return ERROR;
  }
  
  obj_open_link = object_get_open(obj);

  if (link_get_id(l) == obj_open_link)
  {
    return link_set_status(l, OPEN_L);
  }
  
  return ERROR;
}

/**
 * @brief It executes USE command in game.
 *
 * Uses a given object
 *
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_use(Game *game, char *arg)
{
  Object *obj = NULL;
  Id id = NO_ID;
  Obj_type type = UNKNOWN_TYPE;
  Player *player = NULL;
  STATUS st = OK;

  printf("**\nEntrando use\n");

  if (!game || !arg)
  {
    st = ERROR;
    return st;
  }

  obj = game_get_object_byName(game, arg);
  printf("Getting obj\n");

  if (obj == NULL)
  {
    printf("Obj es NULL\n**\n");
    st = ERROR;
    return st;
  }

  printf("Gettin id & player\n");
  id = obj_get_id(obj);
  player = game_get_player(game, 21);
  
  
  if (id == NO_ID || !player)
  {
    printf("Id obj es NO_ID or player == NULL\n**\n");
    st = ERROR;
    return st;
  }

  printf("Viendoo si el player tiene el objeto\n");
  if (inventory_has_id(player_get_inventory(player), id) == FALSE)
  {
    printf("No tiene el objeto\n**\n");
    st = ERROR;
    return st;
  }

  printf("Viendo tipo object\n");
  type = obj_get_type(id);

  if (type == APPLE && st == OK) /*Case apples*/
  {
    printf("Es manzana\n");
    st = inventory_remove_object(player_get_inventory(player), id);
    st = player_set_health(player, player_get_health(player) + 1);

    st = obj_set_location(game_get_object(game, id), -1);

    return st;
  }
  else if (type == ELIXIR && st == OK) /*Case elixir*/
  {
    printf("Es elixir\n");
    st = inventory_remove_object(player_get_inventory(player), id);
    st = player_set_health(player, player_get_health(player) + 2);

    st = obj_set_location(game_get_object(game, id), -1);

    return st;
  }
  else if (type == ARMOR && st == OK) /*Case armour*/
  {
    printf("Es armadura\n");
    st = inventory_remove_object(player_get_inventory(player), id);
    st = player_set_max_health(player, player_get_max_health(player) + 1);

    st = obj_set_location(game_get_object(game, id), -1);

    return st;
  }
  else if (type == HOOK && st == OK) /*Case hook*/
  {
    printf("Es hook\n");
    st = inventory_remove_object(player_get_inventory(player), id);
    if (player_get_location(player) == 125)
    {
      st = obj_set_location(game_get_object(game, 32), -1);
      st = inventory_add_object(player_get_inventory(player), 32);

      st = obj_set_location(game_get_object(game, id), -1);

      return st;
    }
  }
  else if (type == BED && st == OK) /*Case bed*/
  {
    printf("Es cama\n");
    st = inventory_remove_object(player_get_inventory(player), id);
    if (game_get_time(game) == DAY)
    {
      st = game_set_time(game, NIGHT);
    }
    else
    {
      st = game_set_time(game, DAY);
    }

    st = obj_set_location(game_get_object(game, id), -1);

    return st;
  }

  printf("No es ninguno: %d", (int) type);
  st = ERROR;
  return st;  
}

/**
 * @brief It executes MOVE event
 * 
 * Apple or Elixir will appear on the player's room 
 * 
 * @param game pointer to game struct
 * @return OK if event happens, ERROR if not
 */
STATUS game_event_move(Game *game){
  
  int rand_num;
  Object *apple = game_get_object_byName(game, "Apple");
  Object *elixir = game_get_object_byName(game, "Elixir");
  Id apple_loc = game_get_object_location(game, obj_get_id(apple));
  Id elixir_loc = game_get_object_location(game, obj_get_id(elixir));

  /*Events only Occurs at Night*/
  if(game->day_time == DAY){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  /*Generates a random number between 0-1
  This is for moving Elixir or Apple*/
  srand(time(NULL));
  rand_num = rand() % 2;

  /*If elixir is not in the map, cant be moved*/
  if(apple_loc == -1 && elixir_loc == -1){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  if(apple_loc != -1 || rand_num == 0){
    game_set_object_location(game, obj_get_id(apple), game_get_player_location(game, 21));
    dialogue_set_event(game->dialogue, DE_MOVEOBJ);
    return OK;
  }

  if(elixir_loc != -1 || rand_num == 1){
    game_set_object_location(game, obj_get_id(elixir), game_get_player_location(game, 21));
    dialogue_set_event(game->dialogue, DE_MOVEOBJ);
    return OK;
  }

  return game_event_move(game);
}

/**
 * @brief It executes TRAP event
 * 
 * Player losses 1 HP
 * 
 * @param game pointer to game struct
 * @return OK if event happens, ERROR if not
 */
STATUS game_event_trap(Game *game){

  /*Events only Occurs at Night*/
  if(game->day_time == DAY){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  /*Player losses one of HP*/
  player_set_health(game->player[MAX_PLAYERS - 1], (player_get_health(game->player[MAX_PLAYERS - 1]) - 1));

  dialogue_set_event(game->dialogue, DE_TRAP);
  return OK;
}

/**
 * @brief It executes SLIME event
 * 
 * An slime appear on the players location 
 * 
 * @param game pointer to game struct
 * @return OK if event happens, ERROR if not
 */
STATUS game_event_slime(Game *game){
  
  Enemy *slime = game_get_enemy_byName(game, "slime");
  Id slime_loc = enemy_get_location(slime);

  /*Events only Occurs at Night*/
  if(game->day_time == DAY){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  /*In case slime is already dead*/
  if(slime_loc == -1){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  /*Sets the enemy on players location*/
  enemy_set_location(slime, player_get_location(game->player[MAX_PLAYERS - 1]));

  dialogue_set_event(game->dialogue, DE_SLIME);
  return OK;
}

/**
 * @brief It executes DAYNIGHT event
 * 
 * Changes day to night and night to day 
 * 
 * @param game pointer to game struct
 * @return OK if event happens, ERROR if not
 */
STATUS game_event_daynight(Game *game){

  if(game_get_time(game) == DAY){
    dialogue_set_event(game->dialogue, DE_NIGHT);
    game_set_time(game, NIGHT);
  }
  else if(game_get_time(game) == NIGHT){
    dialogue_set_event(game->dialogue, DE_DAY);
    game_set_time(game, DAY);
  }

  return OK;
}

/**
 * @brief It executes SPAWN event
 * 
 * Player tps to entrance
 * 
 * @param game pointer to game struct
 * @return OK if event happens, ERROR if not
 */
STATUS game_event_spawn(Game *game){

  /*Events only Occurs at Night*/
  if(game->day_time == DAY){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }
  
  /*You cannot get lost while attacking*/
  if(game->last_cmd == ATTACK){
    dialogue_set_event(game->dialogue, DE_NOTHING);
    return ERROR;
  }

  /*Sets player to the initial room*/
  player_set_location(game->player[0], SPACE_INITIAL);

  dialogue_set_event(game->dialogue, DE_SPAWN);
  return OK;
}

/*Function that gets the enemy id based on the position it is located in the enemy array located in the game structure */
Id game_get_enemy_id(Game *game, int num)
{

  /* Error control */
  if (!game || num < 0 || num > MAX_ENEMYS - 1)
  {
    return NO_ID;
  }

  return (Id)enemy_get_id(game->enemy[num]);
}

Id game_get_player_id(Game *game)
{

  /* Error control */
  if (!game)
  {
    return NO_ID;
  }

  return (Id)player_get_id(game->player[MAX_PLAYERS - 1]);
}

/** 
 * Function that gets the object id based on the position it is located in 
 * the object array located in the game structure 
 */
Id game_get_object_id(Game *game, int num)
{

  /* Error control */
  if (!game || num < 0 || num > MAX_ENEMYS - 1)
  {
    return NO_ID;
  }

  return (Id)obj_get_id(game->object[num]);
}

Game *game_alloc2()
{

  Game *game = NULL;

  game = (Game *)malloc(sizeof(Game));

  /* Error control */
  if (!game)
  {
    fprintf(stderr, "Error saving memory for game(game_create)");
    return NULL;
  }
  game->inspection = "\0";
  game->day_time = DAY;
  game->last_event = NOTHING;
  game->dialogue = dialogue_create();

  return game;
}
STATUS game_save(char *filename, Game* game)
{
 int i = 0;
 Inventory *player_inventory = player_get_inventory(game_get_player(game, 21));
 FILE *file = NULL;
 file = fopen(filename, "w");
 fclose(file);

  /*Spaces*/
  for (i = 0; i < MAX_SPACES && game->spaces[i] != NULL; i++)
  {
    space_print_save(filename, game->spaces[i]);
  }

  /*Objects*/
  for (i = 0; i < MAX_OBJS && game->object[i] != NULL; i++)
  {
    obj_print_save(filename, game->object[i]);
  }

  /*Players*/
  for (i = 0; i < MAX_PLAYERS && game->player[i] != NULL; i++)
  {
    player_print_save(filename, game->player[i]);
  }

  /*Enemies*/
  for (i = 0; i < MAX_ENEMYS && game->enemy[i] != NULL; i++)
  {
    enemy_print_save(filename, game->enemy[i]);
  }

  /*Links*/
  for (i = 0; i < MAX_LINKS && game->links[i] != NULL; i++)
  {
    link_print_save(filename, game->links[i]);
  }

  /*Inventory*/

  inventory_print_save(filename, player_inventory, 21);

  return OK;
}

Dialogue *game_get_dialogue(Game *game)
{
  if (!game)
  {
    return NULL;
  }

  return game->dialogue;
}

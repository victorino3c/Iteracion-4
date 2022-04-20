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
  char *inspection;            /*!< Long description for inspect space*/
  T_Command last_cmd;          /*!< Last command input */
} ;

/**
   Private functions
*/
int game_command_unknown(Game *game, char *arg);
STATUS game_command_exit(Game *game, char *arg);
STATUS game_command_down(Game *game, char *arg);
STATUS game_command_up(Game *game, char *arg);
STATUS game_command_take(Game *game, char *arg);
STATUS game_command_drop(Game *game, char *arg);
STATUS game_command_attack(Game *game, char *arg);
STATUS game_command_left(Game *game, char *arg);
STATUS game_command_right(Game *game, char *arg);
STATUS game_command_move(Game *game, char *arg);
STATUS game_command_inspect(Game *game, char *arg);

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
 * Finds an object with the same name. Case is ignore.
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

/** game_update updates cmd whenever something is typed in, this is where
 * every known case gets interpreted as the various actions on the game,
 * any other input would be considered unknown
 */
int game_update(Game *game, T_Command cmd, char *arg)
{
  int st = 0;
  
  if(!game){
    return 0;
  }
  
  game->last_cmd = cmd;
  game->inspection = 0;

  switch (cmd)
  {
  case UNKNOWN:
    st = game_command_unknown(game, arg);
    break;

  case EXIT:
    st = (int)game_command_exit(game, arg);
    break;

  case DOWN:
    st = (int)game_command_down(game, arg);
    break;

  case RIGHT:
    st = (int)game_command_right(game, arg);
    break;

  case LEFT:
    st = (int)game_command_left(game, arg);
    break;

  case UP:
    st = (int)game_command_up(game, arg);
    break;

  case TAKE:
    st = (int)game_command_take(game, arg);
    break;

  case DROP:
    st = (int)game_command_drop(game, arg);
    break;

  case ATTACK:
    st = (int)game_command_attack(game, arg);
    break;

  case MOVE:
    st = (int)game_command_move(game, arg);
    break;

  case INSPECT:
    st = (int)game_command_inspect(game, arg);
    break;

  default:
    break;
  }

  return st;
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

  obj_taken = game_get_object_byName(game, arg);
  id_obj_taken = obj_get_id(obj_taken);

  /* Error control*/
  if (space_has_object(game_get_space(game, player_location), id_obj_taken) && set_get_nids(inventory_get_objects(player_get_inventory(game->player[0]))) < inventory_get_maxObjs(player_get_inventory(game->player[0])))
  {
    s = game_get_space(game, player_location);
    o = game_get_object(game, id_obj_taken);
    obj_loc = game_get_object_location(game, id_obj_taken);

    /* Error control*/
    if (player_location != obj_loc)
    {
      return ERROR;
    }

    /* Error control*/
    if (s == NULL)
    {
      return ERROR;
    }

    /* Error control*/
    if (o == NULL)
    {
      return ERROR;
    }

    if (game_set_object_location(game, obj_get_id(o), NO_ID) == ERROR)
    /* Error control */
    {
      return ERROR;
    }

    /* Error control */
    if (player_add_object(game->player[MAX_PLAYERS - 1], o) == ERROR)
    {
      return ERROR;
    }
    /* Error control */
    if (space_del_objectid(s, id_obj_taken) == ERROR)
    {
      return ERROR;
    }
    return OK;
  }

  return ERROR;
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

  Space *s = NULL;
  Object *o = NULL;

  obj = game_get_object_byName(game, arg);
  obj_id = obj_get_id(obj);

  /* Error control */
  if (obj_id != obj_get_id(obj) || inventory_has_id(player_get_inventory(game->player[MAX_PLAYERS - 1]), obj_id) == FALSE)
  {
    return ERROR;
  }

  /* Error control*/
  if (player_get_inventory(game->player[MAX_PLAYERS - 1]) == NULL)
  {
    return ERROR;
  }

  s = game_get_space(game, player_location);
  /* Error control*/
  if (s == NULL)
  {
    return ERROR;
  }

  o = game_get_object(game, obj_id);
  /* Error control*/
  if (o == NULL)
  {
    return ERROR;
  }

  /* Error control */
  if (space_add_objectid(s, obj_get_id(o)) == ERROR)
  {
    return ERROR;
  }

  /* Error control */
  if (game_set_object_location(game, obj_get_id(o), space_get_id(s)) == ERROR)
  {
    return ERROR;
  }
  /* Error control */
  if (player_del_object(game->player[MAX_PLAYERS - 1], obj_id) == ERROR)
  {
    return ERROR;
  }

  return OK;
}

/**
 * @brief It executes DOWN command in game.
 * 
 * When the last command is interpreted as down, move
 * the player to the space South to its current position,
 * updating it afterwards
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_down(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id player_id = player_get_id(game->player[MAX_PLAYERS - 1]);

  Space *s = game_get_space(game, player_location);
  /* S is south */
  Link *l = game_get_link(game, space_get_link(s, S));

  /* Error control */
  if (player_id == NO_ID || player_location == NO_ID)
  {
    return ERROR;
  }

  if (!s)
  {
    return ERROR;
  }

  if (!l)
  {
    return ERROR;
  }

  if (link_get_status(l) == OPEN)
  {
    game_set_player_location(game, player_id, link_get_destination(l));
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/**
 * @brief It executes UP command in game.
 * 
 * When the last command is interpreted as up, move
 * the player to the space North to its current position,
 * updating it afterwards
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_up(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id player_id = player_get_id(game->player[MAX_PLAYERS - 1]);

  Space *s = game_get_space(game, player_location);
  /* N is north */
  Link *l = game_get_link(game, space_get_link(s, N));

  /* Error control */
  if (player_id == NO_ID || player_location == NO_ID)
  {
    return ERROR;
  }

  if (!s)
  {
    return ERROR;
  }

  if (!l)
  {
    return ERROR;
  }

  if (link_get_status(l) == OPEN)
  {
    game_set_player_location(game, player_id, link_get_destination(l));
    return OK;
  }
  else
  {
    return ERROR;
  }
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
  int rand_num = 0;

  Id player_loc = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id enemy_loc = enemy_get_location(game->enemy[MAX_PLAYERS - 1]);

  srand(time(NULL));

  rand_num = rand() % 10;

  /* Error control */
  if (enemy_get_health(game->enemy[MAX_PLAYERS - 1]) == 0)
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
  /*Player wins if rand_num es > 5, else, they lose a life as the enemy won that round*/
  if (rand_num > 5)
  {
    enemy_set_health(game->enemy[MAX_PLAYERS - 1], (enemy_get_health(game->enemy[MAX_PLAYERS - 1]) - 1));
  }
  else
  {
    player_set_health(game->player[MAX_PLAYERS - 1], (player_get_health(game->player[MAX_PLAYERS - 1]) - 1));
    if (player_get_health(game->player[MAX_PLAYERS - 1]) == 0)
    {
      game_is_over(game);
    }
  }

  return OK;
}

/**
 * @brief It executes LEFT command in game.
 * 
 * When the last command is interpreted as left, move
 * the player to the space West to its current position,
 * updating it afterwards
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_left(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id player_id = player_get_id(game->player[MAX_PLAYERS - 1]);

  Space *s = game_get_space(game, player_location);
  /* W is west */
  Link *l = game_get_link(game, space_get_link(s, W));

  /* Error control */
  if (player_id == NO_ID || player_location == NO_ID)
  {
    return ERROR;
  }

  if (!s)
  {
    return ERROR;
  }

  if (!l)
  {
    return ERROR;
  }

  if (link_get_status(l) == OPEN)
  {
    game_set_player_location(game, player_id, link_get_destination(l));
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/**
 * @brief It executes RIGHT command in game.
 * 
 * When the last command is interpreted as right, move
 * the player to the space East to its current position,
 * updating it afterwards
 * 
 * @param game pointer to game struct
 * @param arg string with command argument
 * @return OK if everything goes well or ERROR if there was any mistake
 */
STATUS game_command_right(Game *game, char *arg)
{
  Id player_location = player_get_location(game->player[MAX_PLAYERS - 1]);
  Id player_id = player_get_id(game->player[MAX_PLAYERS - 1]);

  Space *s = game_get_space(game, player_location);
  /* E is east */
  Link *l = game_get_link(game, space_get_link(s, E));

  /* Error control */
  if (player_id == NO_ID)
  {
    return ERROR;
  }

  if (!s)
  {
    return ERROR;
  }

  if (!l)
  {
    return ERROR;
  }

  if (link_get_status(l) == OPEN)
  {
    game_set_player_location(game, player_id, link_get_destination(l));
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/**
 * @brief It executes MOVE command in game.
 * 
 * Another way to move, commands move n,s,e,w
 * to north, south, east or west respectively
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
  STATUS st = ERROR;

  if (strcasecmp(arg, west[0]) == 0 || strcasecmp(arg, west[1]) == 0)
  {
    st = game_command_left(game, NULL);
  }
  else if (strcasecmp(arg, north[0]) == 0 || strcasecmp(arg, north[1]) == 0)
  {
    st = game_command_up(game, NULL);
  }
  else if (strcasecmp(arg, south[0]) == 0 || strcasecmp(arg, south[1]) == 0)
  {
    st = game_command_down(game, NULL);
  }
  else if (strcasecmp(arg, east[0]) == 0 || strcasecmp(arg, east[1]) == 0)
  {
    st = game_command_right(game, NULL);
  }
  else
  {
    st = ERROR;
  }

  return st;
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

  /*SPACE CASE*/
  if (strcmp(arg, "space") == 0 || strcmp(arg, "s") == 0)
  {
    if(space_get_light_status(game_get_space(game, player_get_location(game->player[MAX_PLAYERS - 1]))) == BRIGHT){
      game->inspection = (char *)space_get_long_description(game_get_space(game, player_get_location(game->player[MAX_PLAYERS - 1])));
    }
    else{
      game->inspection = "El lugar está muy oscuro, no puedes ver nada";
    }
    return OK;
  }

  /*OBJECT CASE*/
  else
  {
    if (arg == NULL)
    {
      game->inspection = " ";
      return ERROR;
    }

    if (obj == NULL)
    {
      game->inspection = " ";
      return ERROR;
    }
    if (player_has_object(game->player[0], obj_get_id(obj)) == FALSE && player_get_location(game->player[0]) != obj_get_location(obj))
    {
      return ERROR;
    }
    game->inspection = (char *)obj_get_description(obj);
    return OK;
  }
  return ERROR;
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

  return game;
}

STATUS game_save(char *filename, Game* game)
{
 int i = 0;
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

  for (i = 0; i < inventory_get_maxObjs(player_get_inventory(game->player[0])) && set_get_ids_by_number(inventory_get_objects(player_get_inventory(game->player[0])), i) != NO_ID; i++)
  {
    inventory_print_save(filename, player_get_inventory(game->player[0]), player_get_id(game->player[0]));
  }

  return OK;
}


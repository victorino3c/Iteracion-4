/**
 * @brief It read all game elements from a file.
 *
 * @file game_reader.c
 * @author Miguel Soto, Nicolas Victorino, Antonio Van-Oers, Ignacio Nunez
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../include/game_reader.h"
/*
STATUS game_add_space(Game *game, Space *space);
STATUS game_add_object(Game *game, Object *obj);
STATUS game_add_player(Game *game, Player *p);
STATUS game_add_enemy(Game *game, Enemy *e);
STATUS game_add_link(Game *game, Link *l);
Id game_get_space_id_at(Game *game, int position);
STATUS game_set_player_location(Game *game, Id player_id, Id space_id);
STATUS game_set_object_location(Game *game, Id obj_id, Id space_id);
STATUS game_set_enemy_location(Game *game, Id enemy_id, Id space_id);
*/
/**
 * Funciones privadas
 */
STATUS game_load_spaces(Game *game, char *filename);
STATUS game_load_objs(Game *game, char *filename);
STATUS game_load_players(Game *game, char *filename);
STATUS game_load_enemy(Game *game, char *filename);
STATUS game_load_link(Game *game, char *filename);

GAME_IS_ELEMENT id_type(Id id);

/**
  Game_reader implementacion
*/

/** game_create_from_file  game_create_from_file initializes
 * all of game's members from the filename.dat
 */
STATUS game_create_from_file(Game *game, char *filename)
{

  if (game_create(game) == ERROR)
  {
    return ERROR;
  }

  if (game_load_spaces(game, filename) == ERROR)
  {
    return ERROR;
  }

  if (game_load_objs(game, filename) == ERROR)
  {
    return ERROR;
  }

  if (game_load_players(game, filename) == ERROR)
  {
    return ERROR;
  }

  if (game_load_enemy(game, filename) == ERROR)
  {
    return ERROR;
  }

  if (game_load_link(game, filename) == ERROR)
  {
    return ERROR;
  }

  /* The player and the object are located in the first space */

  return OK;
}

/**
 * @brief Loads the spaces of the game
 * @author Profesores PPROG
 *
 * @param game pointer to the game
 * @param filename pointer to the file from where spaces are being loaded
 * @return OK if everything goes right or ERROR if something goes wrong
 */
STATUS game_load_spaces(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char description[WORD_SIZE] = "";
  char *toks = NULL, *aux;
  char **gdesc = NULL;
  Id id = NO_ID;
  Space *space = NULL;
  STATUS status = OK;
  int i, j;

  if (!filename)
  {
    return ERROR;
  }

  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#s:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      strcpy(description, toks);

      gdesc = space_create_gdesc();
      if (gdesc == NULL)
      {
        return ERROR;
      }

      for (i = 0; i < TAM_GDESC_Y; i++)
      {
        aux = strtok(NULL, "|");
        for (j = 0; j < strlen(aux) && j < TAM_GDESC_X; j++)
        {
          gdesc[i][j] = aux[j];
        }
      }

#ifdef DEBUG
      // printf("Leido: %ld|%s|%s\n", id, name, description);
#endif
      space = space_create(id);
      if (space != NULL)
      {
        space_set_name(space, name);
        space_set_description(space, description);
        space_set_gdesc(space, gdesc);
        game_add_space(game, space);
      }
    }
  }

  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}

/**
 * @brief Loads the objects of the game
 * @author Miguel Soto
 *
 * @param game pointer to game
 * @param filename pointer to the file's name
 * @return OK if everything goes right or ERROR if something goes wrong
 */
STATUS game_load_objs(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char description[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, pos = NO_ID;
  Object *obj = NULL;
  STATUS status = OK;

  if (!filename)
  {
    return ERROR;
  }

  file = fopen(filename, "r");
  /*Error control*/
  if (file == NULL)
  {
    return ERROR;
  }

  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#o:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      strcpy(description, toks);
      toks = strtok(NULL, "|");
      pos = atol(toks);

#ifdef DEBUG
      // printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, north, east, south, west);
#endif
      obj = obj_create(id);
      if (obj != NULL)
      {
        obj_set_name(obj, name);
        obj_set_description(obj, description);
        obj_set_location(obj, pos);
        /*Error control*/
        if (space_add_objectid(game_get_space(game, pos), id) == ERROR)
        {
          return ERROR;
        }
        game_add_object(game, obj);
      }
    }
  }

  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}

/**
 * @brief Loads the player into the game
 * @author Nicolas Victorino
 *
 * @param game pointer to game
 * @param filename pointer to the file from where it is going to load the player
 * @return OK if everything is right ERROR if something went wrong
 */
STATUS game_load_players(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, location = NO_ID;
  int objects, health;
  Player *player = NULL;
  STATUS status = OK;

  /*Error control*/
  if (!filename)
  {
    return ERROR;
  }

  /*Error control*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  /*
  * While the loop reads information in the current line from the file:
  "hormiguero.dat", it divides that line in smaller tokens.
  * Each token has a piece of information, in the following order:
  * ID of the player, name, Id of the object that the player has
  * (NO_ID if it has no object), and location.
  */
  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#p:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      location = atol(toks);
      toks = strtok(NULL, "|");
      health = atol(toks);
      toks = strtok(NULL, "|");
      objects = atol(toks);

      /*If debug is being used, it will print all the information from
        the current player that is being loaded*/
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld|%ld\n", id, name, object, location);
#endif

      /*Defines a private variable called "player" and saves a pointer to player with the given id in it*/
      player = player_create(id);

      /*Error control, and in case everything is fine, it saves the
        information gotten in the prior loop in the newly created player*/
      if (player != NULL)
      {
        player_set_name(player, name);
        player_set_max_inventory(player, objects);
        player_set_location(player, location);
        player_set_health(player, health);
        game_add_player(game, player);
      }
    }
  }

  /*Error control, if it has given an error at any moment while using the file,
  ferror while make the if condition be true.
  This will change the private status variable declared at the beggining
  of the function from OK to ERROR. */
  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}

/**
 * @brief Loads the enemy into the game
 * @author Nicolas Victorino
 *
 * @param game pointer to game
 * @param filename pointer to the file from where it is going to load the enemy
 * @return OK if everything is right ERROR if something went wrong
 */
STATUS game_load_enemy(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, location = NO_ID;
  int health;
  Enemy *enemy = NULL;
  STATUS status = OK;

  /*Error control*/
  if (!filename)
  {
    return ERROR;
  }

  /*Error control*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  /*
   * While the loop reads information in the current line from the file:
   * "hormiguero.dat", it divides that line in smaller tokens.
   * Each token has a piece of information, in the following order:
   * ID of the enemy, name, and location.
   */
  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#e:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      location = atol(toks);
      toks = strtok(NULL, "|");
      health = atol(toks);

      /*If debug is being used, it will print all the information
      from the current enemy that is being loaded*/
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld\n", id, name, location);
#endif

      /*Defines a private variable called "enemy" and saves
      a pointer to player with the given id in it*/
      enemy = enemy_create(id);

      /*Error control, and in case everything is fine, it saves
      the information gotten in the prior loop in the newly created enemy*/
      if (enemy != NULL)
      {
        enemy_set_name(enemy, name);
        enemy_set_location(enemy, location);
        enemy_set_health(enemy, health);
        game_add_enemy(game, enemy);
      }
    }
  }

  /*Error control, if it has given an error at any moment
  while using the file, ferror while make the if condition be true.
   This will change the private status variable declared at
   the beggining of the function from OK to ERROR. */
  if (ferror(file))
  {
    status = ERROR;
  }

  fclose(file);

  return status;
}

/**
 * @brief Loads the links into the game
 * @author Nicolas Victorino
 *
 * @param game pointer to game
 * @param filename pointer to the file from where it is going to load the enemy
 * @return OK if everything is right ERROR if something went wrong
 */
STATUS game_load_link(Game *game, char *filename)
{
  FILE *file = NULL;
  char line[WORD_SIZE] = "";
  char name[WORD_SIZE] = "";
  char *toks = NULL;
  Id id = NO_ID, id_start = NO_ID, id_dest = NO_ID;
  DIRECTION dir = ND;
  LINK_STATUS status = CLOSE;
  Link *link = NULL;
  STATUS st = OK;

  /*Error control*/
  if (!filename)
  {
    return ERROR;
  }

  /*Error control*/
  file = fopen(filename, "r");
  if (file == NULL)
  {
    return ERROR;
  }

  /*
  * While the loop reads information in the current line from the file:
  "hormiguero.dat", it divides that line in smaller tokens.
  * Each token has a piece of information, in the following order:
  * ID of the link, name, and location.
  */
  while (fgets(line, WORD_SIZE, file))
  {
    if (strncmp("#l:", line, 3) == 0)
    {
      toks = strtok(line + 3, "|");
      id = atol(toks);
      toks = strtok(NULL, "|");
      strcpy(name, toks);
      toks = strtok(NULL, "|");
      id_start = atol(toks);
      toks = strtok(NULL, "|");
      id_dest = atol(toks);

      toks = strtok(NULL, "|");
      switch (atoi(toks))
      {
      case 0:
        dir = N;
        break;

      case 1:
        dir = S;
        break;

      case 2:
        dir = E;
        break;

      case 3:
        dir = W;
        break;

      default:
        dir = ND;
        break;
      }

      toks = strtok(NULL, "|");
      switch (atoi(toks))
      {
      case 1:
        status = OPEN;
        break;

      default:
        status = CLOSE;
        break;
      }

      /*If debug is being used, it will print all the information
      from the current link that is being loaded*/
#ifdef DEBUG
      printf("Leido: %ld|%s|%ld|%ld|%ld|%ld\n", id, name, id_start, id_end, dir, status);
#endif

      /*Defines a private variable called "link" and saves a
      pointer to player with the given id in it*/
      link = link_create(id);

      /*Error control, and in case everything is fine, it saves
      the information gotten in the prior loop in the newly created link*/
      if (link != NULL)
      {
        link_set_name(link, name);
        link_set_start(link, id_start);
        link_set_destination(link, id_dest);
        link_set_direction(link, dir);
        link_set_status(link, status);
        space_set_link(game_get_space(game, id_start), id, dir);
        link_print(link);
        game_add_link(game, link);
      }
    }
  }

  /*Error control, if it has given an error at any moment
   while using the file, ferror while make the if condition be true.
   This will change the private status variable declared
   at the beggining of the function from OK to ERROR. */
  if (ferror(file))
  {
    st = ERROR;
  }

  fclose(file);

  return st;
}

/**
 * @brief It indicates which element of the game has the target id
 * @author Miguel Soto
 *
 * @param id target id 
 * @return game elements with the target id
 */
GAME_IS_ELEMENT id_type(Id id)
{
  int first_digit, digits;

  /*Error control*/
  if (id < 0)
  {
    return '\0';
  }

  digits = (int)log10(id);

  first_digit = (int)(id / pow(10, digits));

  if (first_digit == FD_ID_SPACE)
  {
    return IS_SPACE;
  }
  else if (first_digit == FD_ID_PLAYER)
  {
    return IS_PLAYER;
  }
  else if (first_digit == FD_ID_OBJ)
  {
    return IS_OBJECT;
  }
  else if (first_digit == FD_ID_ENEMY)
  {
    return IS_ENEMY;
  }
  /*Error control*/
  else
  {
    return '\0';
  }
}

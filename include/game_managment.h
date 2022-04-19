/** 
 * @brief It defines the game's members loading
 * 
 * @file game_reader.h
 * @author Miguel Soto, Nicolas Victorino, Ignacio Nunez, Antonio Van-Oers
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#ifndef GAME_READER
#define GAME_READER

#include "command.h"
#include "space.h" 
#include "types.h"
#include "game.h"

/**
 * @brief It is used to classify ids as a game element which could be any type from a space or player, to an object or enemy
 */
typedef enum
{
  IS_SPACE = FD_ID_SPACE,   /*!< Game element is a space */
  IS_PLAYER = FD_ID_PLAYER, /*!< Game element is a player */
  IS_OBJECT = FD_ID_OBJ,    /*!< Game element is an object */
  IS_ENEMY = FD_ID_ENEMY    /*!< Game element is an enemy */
} GAME_IS_ELEMENT;  

/**
  * @brief Creates and initializes a game from a file
  * @author Profesores PPROG
  *
  * game_create_from_file initializes all of game's members from the filename.dat
  * @param game pointer to game
  * @param filename pointer to the file's name 
  * @return OK, if everything goes well or ERROR if something didn't
  */
STATUS game_create_from_file(Game *game, char *filename);

#endif

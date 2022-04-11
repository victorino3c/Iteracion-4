/** 
 * @brief Implements the player's interface
 * 
 * @file player.h
 * @author Antonio Van-Oers, Nicolas Victorino, Ignacio Nunez and Miguel Soto
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"
#include "object.h"
#include "inventory.h"

#define PLAYER_NAME_LEN 60   /*!< Establish player's name length */

typedef struct _Player Player; /*!< It defines the player structure, it can move, attack or take and drop objects */

/**
 * @brief Creates a new player.
 * @author Antonio Van-Oers 
 *
 * player_create player_create allocates memory for a new player
 * and initializes all its members
 * @param id is the target player's id
 * @return a new, initialized player, or NULL if anything goes wrong.
 */
Player *player_create(Id id);

/**
 * @brief Frees all of a player's allocated memory
 * @author Antonio Van-Oers 
 * 
 * player_destroy frees all previously allocated
 * memory for a player
 * @param player a pointer to target player
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_destroy(Player *player);

/**
 * @brief Gets a player's id
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @return player's id if everything goes well, or NULL if anything goes wrong.
 */
Id player_get_id(Player *player);

/**
 * @brief Tests whether an id is from a player or not
 * @author Miguel Soto
 * 
 *@param id is the target id
 * @return OK, if everything goes well, or ERROR if anything goes wrong.
 */
STATUS player_test_id(Id id);

/**
 * @brief Gets a player's name
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @return a string with the plyer's name, or NULL if anything goes wrong.
 */
const char *player_get_name(Player *player);

/**
 * @brief Gets a player's location
 * @author Antonio Van-Oers
 * 
 * player_get_location obtiene la localizacion de un jugador (player).
 * @param player a pointer to target player
 * @return player->location, player's location id or NO_ID if anything goes wrong
 */
Id player_get_location(Player *player);

/**
 * @brief Gets a player's array of objects
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @return player->inventory, an inventory type variable from the 
 * player or NULL if anything goes wrong.
 */
Inventory *player_get_inventory(Player *player);

/**
 * @brief Gets a player's health.
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @return player->health, the remainig health on the 
 * target player or -1 if anything goes wrong.
 */
int player_get_health(Player *player);

/**
 * @brief Sets a player's health to the assigned value.
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @param health indicates the amount of health to be given to the player
 * @return OK if everything goes well or ERROR if anything goes wrong.
 */
STATUS player_set_health(Player *player, int health);

/**
 * @brief Assigns an object to a player
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @param object the object about to be added to the player's belongings
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_add_object(Player *player, Object *object);

/**
 * @brief Removes an object from a player's inventory
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @param id_obj id to the object about to be removed from the player's belongings
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_del_object(Player *player, Id id_obj);


/**
 * @brief Assigns the maximum number of objects available in the inventory
 * @author Nicolas Victorino
 * 
 * @param player a pointer to target player
 * @param num new maximum number of objects the player can have
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_set_max_inventory(Player* player, int num);
/**
 * @brief Sets a player's location
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @param location is the space's id on which the player is located
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_set_location(Player *player, Id location);

/**
 * @brief Sets the player's new name
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @param name a string with the new name for the player
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_set_name(Player *player, char *name);

/**
 * @brief If a player has an object or not
 * @author Ignacio Nunnez
 * 
 * @param player a pointer to target player
 * @param obj_id id of the object to find
 * @return TRUE if player has it, FALSE if not or there was a mistake
 */
BOOL player_has_object(Player *player, Id obj_id);

/**
 * @brief Prints all player related info in a string
 * @author Antonio Van-Oers
 * 
 * @param player a pointer to target player
 * @return OK, if the task was successfully completed or ERROR, if anything goes wrong.
 */
STATUS player_print(Player *player);

#endif


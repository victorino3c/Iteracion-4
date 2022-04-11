/** 
 * @brief It implements the enemy interface.
 * 
 * @file enemy.h
 * @author Antonio Van-Oers, Ignacio Nunez, Nicolas VIctorino and Miguel Soto
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

#define ENEMY_LEN_NAME 60  /*!< Enemy's name length */

/**
 * @brief Enemy
 *
 * This structure stores all info that each enemy has
 */
typedef struct _Enemy Enemy;


/**
 * @brief Creates a new enemy.
 * @author Antonio Van-Oers 
 *
 * enemy_create allocates memory for a new enemy and initializes all its members .
 * @param id new enemy's identification number.
 * @return a new enemy, initialized, or NULL in case of error.
 */
Enemy *enemy_create(Id id);

/**
 * @brief It destroys an enemy
 * @author Antonio Van-Oers 
 * 
 * enemy_destroy frees the previous memory allocation for an enemy
 * @param enemy a pointer to the enemy that must be destroyed.
 * @return OK, if everything goes well o ERROR, if there was some mistake.
 */
STATUS enemy_destroy(Enemy *enemy);

/**
 * @brief It gets an enemy's id
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * @return enemy id, or NULL if there was some mistake.
 */
Id enemy_get_id(Enemy *enemy);

/**
 * @brief Tests whether the id is from an enemy or not
 * @author Miguel Soto
 * 
 * @param id to be tested
 * @return OK, if it is indeed from an enemy, ERROR if it is not or anything goes wrong. 
 */
STATUS enemy_test_id(Id id);

/**
 * @brief It gets an enemy's name
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy.
 
 * @return a string with the enemy's name, or NULL if anything goes wrong.
 */
const char *enemy_get_name(Enemy *enemy);

/**
 * @brief It gets an enemy's location
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * @return enemy->location, enemy's location id or NO_ID if anything goes wrong.
 */
Id enemy_get_location(Enemy *enemy);

/**
 * @brief It gets an enemy's health
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * @return enemy->health, enemy's remaining health
 * and -1 if anything goes wrong.
 */
int enemy_get_health(Enemy *enemy);

/**
 * @brief It sets an enemy's health
 * @author Antonio Van-Oers
 * 
 * enemy_set_health sets an enemy's helath to the value indicated.
 * @param enemy a pointer to a certain enemy.
 * @param health an integer with the amount of health.
 * return OK, if everything goes well o ERROR, if there was some mistake.
 */
STATUS enemy_set_health(Enemy *enemy, int health);

/**
 * @brief It sets an enemy's location.
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * @param location is an enemy's location id.
 *return OK, if everything goes well o ERROR, if there was some mistake.
 */
STATUS enemy_set_location(Enemy *enemy, Id location);

/**
 * @brief It sets an enemy's name
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * @param name un string with the name for the enemy
 * return OK, if everything goes well o ERROR, if there was some mistake.
 */
STATUS enemy_set_name(Enemy *enemy, char *name);

/**
 * @brief Prints the info on the enemy's interface
 * @author Antonio Van-Oers
 * 
 * @param enemy a pointer to a certain enemy
 * return OK, if everything goes well o ERROR, if there was some mistake.
 */
STATUS enemy_print(Enemy *enemy);

#endif

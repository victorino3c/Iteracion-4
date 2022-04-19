/** 
 * @brief Implements the enemy module.
 * 
 * @file enemy.c
 * @author Antonio Van-Oers, Ignacio Nunez, Nicolas Victorino and Miguel Soto.
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/enemy.h"
#include "../include/types.h"

/**
 * @brief Enemy
 *
 * This structure stores all info that each enemy has
 */
struct _Enemy
{
  Id id;  /*!< Enemy's id. */
  char name[ENEMY_LEN_NAME]; /*!< Enemy's name. */
  Id location;  /*!< Id to enemy's space location */
  int health;   /*!< Enemy's health */
} ;

/**enemy_create allocates memory for a new enemy and initializes all its members .
 */
Enemy *enemy_create(Id id)
{
  Enemy *new_enemy = NULL;

  /* Error control */
  if (id == NO_ID)
  {
      return NULL;
  }

  new_enemy = (Enemy *) malloc(sizeof(Enemy));
	/* Error control */
  if (new_enemy == NULL)
  {
    return NULL;
  }
  
  /* Enemy's members initialization*/
  new_enemy->id = id;
  new_enemy->health = 3;
  new_enemy->name[0] = '\0';
  new_enemy->location = NO_ID;

  return new_enemy;
}

/** enemy_destroy frees the previous memory allocation for an enemy
 */
STATUS enemy_destroy(Enemy *enemy)
{
	/* Error control */
  if (!enemy)
  {
    return ERROR;
  }
  
  free(enemy);
  enemy = NULL;
  return OK;
}

/**  It gets an enemy's id
 */
Id enemy_get_id(Enemy *enemy)
{
	/* Error control */
  if (!enemy)
  {
    return NO_ID;
  }
  
  return enemy->id;
}

/**
 * Tests whether the id is from an enemy or not
 */
STATUS enemy_test_id(Id id)
{
  int first_digit, digits;

  /* Error control */
  if (id < 0)
  {
    return ERROR;
  }

  /* Calculates the total digits -1 */
  digits = (int)log10(id); 

  /* Getting the first digit */
  first_digit = (int)(id / pow(10, digits));
  
  if (first_digit == FD_ID_ENEMY)
  {
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/** enemy_get_name  gets an enemy's name.
 */
const char *enemy_get_name(Enemy *enemy)
{
	/* Error control */
  if (!enemy)
  {
    return NULL;
  }
  
  return enemy->name;
}

/** enemy_get_location  gets an enemy's location.
 */
Id enemy_get_location(Enemy *enemy)
{
	/* Error control */
  if (!enemy)
  {
    return NO_ID;
  }
  
  return enemy->location;
}

/**
 * enemy_get_health It gets an enemy's health.
 */
int enemy_get_health(Enemy *enemy)
{
  /* Error control */
  if (!enemy)
  {
    return -1;                  
  }
  
  return enemy->health;
}


/**
 * enemy_set_health sets an enemy's health.
 */
STATUS enemy_set_health(Enemy *enemy, int health)
{
	/* Error control */
  if (!enemy)
  {
    return ERROR;
  }

  if(health<0){
    return ERROR;
  } 
  
  enemy->health = health;
  return OK;
}
 

/** enemy_set_name sets an enemy's location.
 */
STATUS enemy_set_location(Enemy *enemy, Id location)
{
	/* Error control */
  if (!enemy)
  {
    return ERROR;
  }
  
  enemy->location = location;
  return OK;
}


/** enemy_set_name  sets an enemy's name.
 */
STATUS enemy_set_name(Enemy *enemy, char *name)
{
	/* Error control */
  if (!enemy || !name)
  {
    return ERROR;
  }
  
  if (strlen(name) >= ENEMY_LEN_NAME)
  {
    strncpy(enemy->name, name, ENEMY_LEN_NAME);
  }
  else
  {
    strcpy(enemy->name, name);
  }
  
  return OK;
}


/** enemy_print  Prints the info on the enemy's interface.
 */
STATUS enemy_print(Enemy *enemy)
{
 /* Error control */
  if (!enemy)
  {
      return ERROR;
  }
  
  fprintf(stdout, "--> enemy (Id: %ld; Name: %s)\n", enemy->id, enemy->name);

  return OK;
}

/** enemy_print  Prints the info on the enemy's interface in the save file
 */
STATUS enemy_print_save(char *filename, Enemy *enemy)
{
  FILE *file = NULL;

  file = fopen(filename, "a");

 /* Error control */
  if (!enemy || !file)
  {
      return ERROR;
  }

  fprintf(file, "e:%ld|%s|%ld|%d|\n", enemy->id, enemy->name, enemy->location, enemy->health);

  fclose(file);

  return OK;
}
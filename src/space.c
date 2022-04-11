/**
 * @brief It implements the space module
 *
 * @file space.c
 * @author Miguel Soto, Antonio Van-Oers, Nicolas Victorino and Ignacio Nunez
 * @version 2.0
 * @date 02-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/space.h"

#define MAX_LINKS_SPACE 4   /*!< Establish the maximun of links per space. This number is related to the number of the different existing direction (declared in types.h)*/

/**
 * @brief Space
 *
 * This struct stores all the information of a space.
 */
struct _Space
{
  Id id;                       /*!< Id number of the space, it must be unique */
  char name[WORD_SIZE + 1];    /*!< Name of the space */
  char description[WORD_SIZE]; /*!< Description of the space*/
  Id link[MAX_LINKS_SPACE];    /*!< Id from links between space with this space as origin */
  Set *objects;                /*!< Conjunto de ids de los objetos que se encuentran en el espacio */
  char **gdesc;                /*!< Array de 5 strings de 9 caracteres */
} ;

/**
 * Private functions
 */
int _dir2i(DIRECTION dir);

/**
 * @brief Given a direction returns position of links array
 * @author Miguel Soto
 *
 * LINK ARRAY INDEXES:
 * (NORTH) link[0];
 * (EAST) link[1];
 * (SOUTH) link[2];
 * (WEST) link[3];
 *
 * @param dir direccion
 * @return index input in link array
 */
int _dir2i(DIRECTION dir)
{
  int n;

  if (dir == N)
  {
    n = 0;
  }
  else if (dir == E)
  {
    n = 1;
  }
  else if (dir == S)
  {
    n = 2;
  }
  else if (dir == W)
  {
    n = 3;
  }
  else
  {
    n = -1;
  }

  return n;
}

/** space_create allocates memory for a new space
 *  and initializes its members
 */
Space *space_create(Id id)
{
  Space *newSpace = NULL;
  int i;

  /* Error control */
  if (id == NO_ID)
    return NULL;

  newSpace = (Space *)malloc(sizeof(Space));
  if (newSpace == NULL)
  {
    return NULL;
  }

  /* Initialization of an empty space*/
  newSpace->id = id;
  newSpace->name[0] = '\0';
  newSpace->description[0] = '\0';
  for (i = 0; i < 4; i++)
  {
    newSpace->link[i] = NO_ID;
  }
  newSpace->objects = set_create();
  newSpace->gdesc = NULL;

  return newSpace;
}

/** space_destroy frees the previous memory allocation
 *  for a space
 */
STATUS space_destroy(Space *space)
{
  int i;

  /* Error control */
  if (!space)
  {
    return ERROR;
  }

  for (i = 0; i < 4; i++)
  {
    space->link[i] = NO_ID;
  }

  if (space->objects)
  {
    if (!set_destroy(space->objects))
    {
      return ERROR;
    }
    space->objects = NULL;
  }

  if (space->gdesc)
  {
    if (space_destroy_gdesc(space->gdesc) == ERROR)
    {
      /*printf("ERROR liberando memoria gdesc\n");*/
      return ERROR;
    }
  }

  free(space);
  space = NULL;
  return OK;
}

/** It gets the id of a space
 */
Id space_get_id(Space *space)
{
  if (!space)
  {
    return NO_ID;
  }
  return space->id;
}

/**
 * Checks if the id reciven corresponds with a space
 */
STATUS space_test_id(Id id)
{
  int first_digit, digits;

  /*Error control*/
  if (id < 0)
  {
    return ERROR;
  }

  /* Calculates the total number of digits -1*/
  digits = (int)log10(id);

  /* Obtaines first digit*/
  first_digit = (int)(id / pow(10, digits));

  if (first_digit == FD_ID_SPACE)
  {
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/** It sets the name of a space
 */
STATUS space_set_name(Space *space, char *name)
{
  if (!space || !name)
  {
    return ERROR;
  }

  if (strlen(name) >= WORD_SIZE)
  {
    strncpy(space->name, name, WORD_SIZE);
  }
  else
  {
    strcpy(space->name, name);
  }
  return OK;
}

/** It gets the name of a space
 */
const char *space_get_name(Space *space)
{
  if (!space)
  {
    return NULL;
  }
  return space->name;
}

/** space_get_description returns the description of a space.
 */
const char *space_get_description(Space *space)
{
  /*CONTROL ERRORS*/
  if (!space)
  {
    return NULL;
  }

  return space->description;
}

/** space_set_description set a new description for an space.
 */
STATUS space_set_description(Space *space, char *description)
{
  /* CONTROL ERROR */
  if (!space || !description)
  {
    return ERROR;
  }

  if (!strcpy(space->description, description))
  {
    /*CONTROL ERROR*/
    return ERROR;
  }

  return OK;
}

/**
 * It sets the link of the space in an specific direction
 */
STATUS space_set_link(Space *space, Id link, DIRECTION dir)
{
  int n = _dir2i(dir);

  if (!space || !link || dir == ND)
  {
    return ERROR;
  }

  if (n < 0)
  {
    return ERROR;
  }

  space->link[n] = link;
  return OK;
}

/**
 * It gets link's id that starts in this space with an specific direction
 */
Id space_get_link(Space *space, DIRECTION dir)
{
  int n = _dir2i(dir);

  if (!space || dir == ND)
  {
    return -1;
  }

  if (n < 0)
  {
    return -1;
  }

  return space->link[n];
}

/**
 * It add an object id in the space's set of objects
 */
STATUS space_add_objectid(Space *s, Id id)
{
  /* Error control*/
  if (!s || !id)
  {
    return ERROR;
  }

  if (!set_add(s->objects, id))
  {
    return ERROR;
  }

  return OK;
}

/**
 * It deletes an object id in the space's set of objects
 */
STATUS space_del_objectid(Space *s, Id id)
{
  /* Error control*/
  if (!s || !id)
  {
    return ERROR;
  }

  if (!set_del_id(s->objects, id))
  {
    return ERROR;
  }

  return OK;
}

/**
 * It gets a pointer to the set of objects in a space
 */
Set *space_get_objects(Space *s)
{
  /* Error control*/
  if (!s)
  {
    return NULL;
  }
  return s->objects;
}

/**
 * It gets a pointer to the id array of a set of objects that are in that space
 */
Id *space_get_objects_ids(Space *s)
{
  /* Error control*/
  if (!s)
  {
    return NULL;
  }

  return set_get_ids(space_get_objects(s));
}

/**
 * It sets the graphic description of a spaces.
 */
BOOL space_has_object(Space *s, Id id)
{
  Id *ids = NULL;
  BOOL b = FALSE;
  int i;

  /* Error control*/
  if (!s)
  {
    return FALSE;
  }

  ids = set_get_ids(s->objects);
  if (ids == NULL)
  {
    return FALSE;
  }

  for (i = 0; (i < set_get_nids(s->objects)) && (b == FALSE); i++)
  {
    if (*(ids + i) == id)
    {
      b = TRUE;
    }
  }

  return b;
}

char **space_create_gdesc()
{
  char **newgdesc = NULL;
  int i, j;

  newgdesc = (char **)malloc((TAM_GDESC_Y + 1) * sizeof(char *));
  if (!newgdesc)
  {
    return NULL;
  }
  for (i = 0; i < (TAM_GDESC_Y + 1); i++)
  {
    newgdesc[i] = NULL;
    newgdesc[i] = (char *)malloc((TAM_GDESC_X + 2) * sizeof(char));
    if (!newgdesc[i])
    {
      return NULL;
    }

    for (j = 0; j < (TAM_GDESC_X + 2); j++)
    {
      newgdesc[i][j] = '\0';
    }
  }

  return newgdesc;
}

STATUS space_destroy_gdesc(char **gdesc)
{
  int i;

  /* Error control*/
  if (!gdesc)
  {
    return ERROR;
  }

  if (gdesc)
  {
    for (i = TAM_GDESC_Y; i >= 0; i--)
    {
      if (gdesc[i])
      {
        free(gdesc[i]);
        gdesc[i] = NULL;
      }
    }

    free(gdesc);
    gdesc = NULL;
  }

  return OK;
}

/**
 * It gets the graphic description from a space.
 */
STATUS space_set_gdesc(Space *s, char **newgdesc)
{
  /* Error control*/
  if (!s || !newgdesc)
  {
    return ERROR;
  }

  s->gdesc = newgdesc;

  return OK;
}

char **space_get_gdesc(Space *s)
{
  /* Error control*/
  if (!s)
  {
    return NULL;
  }

  return s->gdesc;
}

/** It prints the space information
 */
STATUS space_print(Space *space)
{
  Id idaux = NO_ID;
  int nobj, i, j;

  /* Error Control */
  if (!space)
  {
    return ERROR;
  }

  /* 1. Print the id and the name of the space */
  fprintf(stdout, "--> Space (Id: %ld; Name: %s)\n", space->id, space->name);

  /* 2. For each direction, print its link */
  idaux = space_get_link(space, N);
  if (NO_ID != idaux)
  {
    fprintf(stdout, "---> North link: %ld.\n", idaux);
  }
  else
  {
    fprintf(stdout, "---> No north link.\n");
  }
  idaux = space_get_link(space, S);
  if (NO_ID != idaux)
  {
    fprintf(stdout, "---> South link: %ld.\n", idaux);
  }
  else
  {
    fprintf(stdout, "---> No south link.\n");
  }
  idaux = space_get_link(space, E);
  if (NO_ID != idaux)
  {
    fprintf(stdout, "---> East link: %ld.\n", idaux);
  }
  else
  {
    fprintf(stdout, "---> No east link.\n");
  }
  idaux = space_get_link(space, W);
  if (NO_ID != idaux)
  {
    fprintf(stdout, "---> West link: %ld.\n", idaux);
  }
  else
  {
    fprintf(stdout, "---> No west link.\n");
  }

  /* 3. Print if there is an object in the space or not */
  nobj = set_get_nids(space_get_objects(space));
  if (nobj > 0)
  {
    fprintf(stdout, "---> %d objects in the space.\n", nobj);
  }
  else
  {
    fprintf(stdout, "---> No object in the space.\n");
  }

  /* 4. Print gdesc*/
  printf("=> Gdesc:\n");
  for (i = 0; i < TAM_GDESC_Y && space->gdesc[i]; i++)
  {
    for (j = 0; j < TAM_GDESC_X && space->gdesc[i][j]; j++)
    {
      printf("%c", space->gdesc[i][j]);
    }
    printf("\n");
    if (j != TAM_GDESC_X)
    {
      printf("ERROR IMPRIMIENDO space->gdesc[%d][%d] = NULL\n", i, j);
    }
  }

  return OK;
}

Id space_get_id_dest_by_link (Link *l)
{
  /*Error control*/
  if (!l)
  return -1;

  return (Id)link_get_destination(l);

}

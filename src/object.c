  /**
 * @brief It implements the object module.
 *
 * @file object.c
 * @author Miguel Soto, Antonio Van-Oers, Nicolas Victorino & Ignacio Nunnez
 * @version 3.0
 * @date 02-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/object.h"

/**
 * @brief Object
 *
 * Esta estructura almacena la informacion de un objeto.
 */
struct _Object
{
  Id id;    /*!< Object's id */
  char name[OBJ_NAME_LEN];  /*!< Object name */
  char description[WORD_SIZE];  /*!< Object description */
  Id location;  /*!< Object space location */
} ;

/** obj_create reserva memoria para un nuevo objeto e inicializa sus miembros.
 */
Object *obj_create(Id id)
{
  Object *new_obj = NULL;

  /* Control de errores */
  if (id == NO_ID)
  {
    return NULL;
  }

  new_obj = (Object *)malloc(sizeof(Object));
  if (new_obj == NULL)
  {
    return NULL;
  }

  /* Inicializacion del objeto nuevo */
  new_obj->id = id;
  new_obj->name[0] = '\0';
  new_obj->description[0] = '\0';
  return new_obj;
}

/** obj_destroy libera la memoria previamente reservada de un objeto.
 */
STATUS obj_destroy(Object *obj)
{
  /* Control de errores */
  if (!obj)
  {
    return ERROR;
  }

  free(obj);
  obj = NULL;
  return OK;
}

/**
 * Comprueba si el id recibido es el de un objeto
 */
STATUS obj_test_id(Id id)
{
  int first_digit, digits;

  /* Control de errores */
  if (id < 0)
  {
    return ERROR;
  }

  /* Calcular numbero total de digitos - 1 */
  digits = (int)log10(id);

  /* Obtener primer digito */
  first_digit = (int)(id / pow(10, digits));

  if (first_digit == FD_ID_OBJ)
  {
    return OK;
  }
  else
  {
    return ERROR;
  }
}

/** obj_get_id devuelve el id de un Object (obj).
 */
Id obj_get_id(Object *obj)
{
  /* Control de errores */
  if (!obj)
  {
    return NO_ID;
  }

  return obj->id;
}

/** obj_get_name devuelve el nombre (name) de un Object (obj).
 */
const char *obj_get_name(Object *obj)
{
  /* Control de errores */
  if (!obj)
  {
    return NULL;
  }

  return obj->name;
}

/** obj_set_name establece el nombre (name) de un Object (obj).
 */
STATUS obj_set_name(Object *obj, char *name)
{
  /* Control de errores */
  if (!obj || !name)
  {
    return ERROR;
  }

  if (strlen(name) >= OBJ_NAME_LEN)
  {
    strncpy(obj->name, name, OBJ_NAME_LEN);
  }
  else
  {
    strcpy(obj->name, name);
  }

  return OK;
}

/** obj_get_description returns the description of an object.
 */
const char *obj_get_description(Object *obj)
{
  /*CONTROL ERRORS*/
  if (!obj)
  {
    return NULL;
  }

  return obj->description;
}

/** obj_set_description set a new description for an object.
 */
STATUS obj_set_description(Object *obj, char *description)
{
  /* CONTROL ERROR */
  if (!obj || !description)
  {
    return ERROR;
  }

  if (!strcpy(obj->description, description))
  {
    /*CONTROL ERROR*/
    return ERROR;
  }

  return OK;
}

Id obj_get_location(Object *obj)
{
  /* Control de errores */
  if (!obj)
  {
    return NO_ID;
  }

  return obj->location;
}

STATUS obj_set_location(Object *obj, Id id)
{
  /* Control de errores */
  if (!obj)
  {
    return ERROR;
  }

  obj->location = id;
  return OK;
}

/** obj_print muestra el id y el nombre de un Object (obj).
 */
STATUS obj_print(Object *obj)
{
  /* Control de errores */
  if (!obj)
  {
    return ERROR;
  }

  fprintf(stdout, "--> Object (Id: %ld; Name: %s)\n", obj->id, obj->name);

  return OK;
}

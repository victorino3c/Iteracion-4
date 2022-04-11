/** 
 * @brief Implements the object's interface
 * 
 * @file object.h
 * @author Miguel Soto, Antonio Van-Oers, Ignacio Nunez and Nicolas Victorino
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#ifndef OBJECT_H
#define OBJECT_H

#include "types.h"

#define OBJ_NAME_LEN 40   /*!< Establish maximun length of object's name */

typedef struct _Object Object;  /*!< It defines the object structure, which can be taken or dropped by the player */


/**
 * @brief Crea un objeto nuevo.
 * @author Miguel Soto
 *
 * obj_create allocates memory for a new object and initializes all its members
 * @param id is the target object's id
 * @return a new object (type Object) initialized, or NULL if anything goes wrong.
 */
Object *obj_create(Id id);

/**
 * @brief Destruye un objeto.
 * @author Miguel Soto
 * 
 * obj_destroy frees the previously allocated memory for an object
 * @param obj is a pointer to the target object.
 * @return Ok if it succesfully completed the task or ERROR, if anything goes wrong.
 */
STATUS obj_destroy(Object *obj);

/**
 * @brief Tests whether the designated id is from an object or not
 * @author Miguel Soto
 * 
 * @param id is the target object's id
 * @return Ok if it succesfully completed the task or ERROR, if anything goes wrong.
 */
STATUS obj_test_id(Id id);

/**
 * @brief  Gets an object's id
 * @author Miguel Soto
 * 
 * @param obj is a pointer to the target object.
 * @return object's id, or NULL if anything goes wrong.
 */
Id obj_get_id(Object *obj);

/**
 * @brief Gets an object's name 
 * @author Miguel Soto
 *
 * @param obj is a pointer to the target object.
 * @return a string with the object's name, or NULL if anything goes wrong.
 */
const char *obj_get_name(Object *obj);

/**
 * @brief Sets an object's name
 * @author Miguel Soto
 * 
 * @param obj is a pointer to the target object.
 * @param name a string containing the new object's name
 * @return Ok if it succesfully completed the task or ERROR, if anything goes wrong.
 */
STATUS obj_set_name(Object *obj, char *name);

/**
 * @brief Obtiene la descripcion de un objeto.
 * @author Ignacio Nunnez
 * 
 * description_get_name devuelve la descripcion de un Object (obj).
 * @param obj un puntero a Object.
 * @return un string con la descripcion del objeto obj, o NULL si hubo algun error.
 */
const char *obj_get_description(Object *obj);

/**
 * @brief Establece una descripcion a un objeto.
 * @author Miguel Soto
 * 
 * obj_set_descripcion establece la descripcion (description)) de un Object (obj).
 * @param obj un puntero a Object.
 * @param description un string con la descripcion que se va a establecer al objeto.
 * @return OK, si ha funcionado correctamente o ERROR, si hubo algun error.
 */
STATUS obj_set_description(Object *obj, char *descripcion);

/**
 * @brief Gets an object's position
 * @author Miguel Soto
 * 
 * @param obj is a pointer to the target object.
 * @return space's id where the object is located.
 */
Id obj_get_location(Object *obj);

/**
 * @brief Sets an object's position
 * @author Miguel Soto
 * 
 * @param obj is a pointer to the target object.
 * @param id is the target object's id
 * @return Ok if it succesfully completed the task or ERROR, if anything goes wrong. 
 */
STATUS obj_set_location(Object *obj, Id id);

/**
 * @brief Prints the info regarding an object 
 * @author Miguel Soto
 * 
 * obj_print prints an object's name and id
 * @param obj is a pointer to the target object.
 * @return Ok if it succesfully completed the task or ERROR, if anything goes wrong.
 */
STATUS obj_print(Object *obj);

#endif

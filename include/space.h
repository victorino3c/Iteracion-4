/**
 * @brief It defines the space interface
 *
 * @file space.h
 * @author Miguel Soto, Antonio Van-Oers, Nicolas Victorino and Ignacio Nunez
 * @version 3.0
 * @date 04-04-2022
 * @copyright GNU Public License
 */

#ifndef SPACE_H
#define SPACE_H

#include "set.h"
#include "types.h"
#include "object.h"
#include "link.h"

typedef struct _Space Space; /*!< The space structure, contains everything related to the game's spaces */

#define FIRST_SPACE 1 /*!< Establish the position of the first space  */
#define TAM_GDESC_Y 5 /*!< Establish the maximun number of litednes space's graphic description (gdesc) will have  */
#define TAM_GDESC_X 9 /*!< Establish the maximun number of characters each line of space's graphic description (gdesc) will have */

/**
 * @brief It creates a new space
 * @author Profesores PPROG
 *
 * space_create allocates memory for a new space
 *  and initializes its members
 *
 * @param id the identification number for the new space
 * @return a new space, initialized
 */
Space *space_create(Id id);

/**
 * @brief It destroys a space
 * @author Profesores PPROG
 *
 * space_destroy frees the previous memory allocation
 *  for a space
 * @param space a pointer to the space that must be destroyed
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_destroy(Space *space);

/**
 * @brief It gets the id of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return the id of space
 */
Id space_get_id(Space *space);

/**
 * @brief Comprueba si el id recibido es el de un espacio
 * @author Miguel Soto
 *
 * @param id el id que se quiere comprobar
 * @return OK, si es correcto o ERROR si no es correcto o hubo algun error.
 */
STATUS space_test_id(Id id);

/**
 * @brief It sets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @param name a string with the name to store
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_set_name(Space *space, char *name);

/**
 * @brief It gets the name of a space
 * @author Profesores PPROG
 *
 * @param space a pointer to the space
 * @return  a string with the name of the space
 */
const char *space_get_name(Space *space);

/**
 * @brief It sets the description of a space
 * @author Ignacio Nunnez
 *
 * @param space a pointer to the space
 * @param description a string with the description to store
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_set_description(Space *space, char *description);

/**
 * @brief It gets the description of a space
 * @author Ignacio Nunnez
 *
 * @param space a pointer to the space
 * @return  a string with the description of the space
 */
const char *space_get_description(Space *space);

/**
 * @brief It sets the link of the space in an specific direction
 * @author Miguel Soto
 *
 * @param space pointer to space
 * @param link id of the link that need to be added into space
 * @param dir link's direction
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_set_link(Space *space, Id link, DIRECTION dir);

/**
 * @brief It gets link's id that starts in this space with an specific direction
 * @author Miguel Soto
 *
 * @param space pointer to space
 * @param dir link's direction
 * @return Id link in space in dir direction.
 */
Id space_get_link(Space *space, DIRECTION dir);

/**
 * @brief It add an object id in the space's set of objects
 * @author Miguel Soto
 *
 * @param s a pointer to the space
 * @param id from an object that wants to be added.
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_add_objectid(Space *s, Id id);

/**
 * @brief It deletes an object id in the space's set of objects
 * @author Miguel Soto
 *
 * @param s a pointer to the space
 * @param id from an object that wants to be added.
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_del_objectid(Space *s, Id id);

/**
 * @brief It gets a pointer to the set of objects in a space
 * @author Miguel Soto
 *
 * @param s a pointer to the space
 * @return a pointer to Set with all objects id that are in that space or NULL if there was an error
 */
Set *space_get_objects(Space *s);

/**
 * @brief It gets a pointer to the id array of a set of objects that are in that space
 * @author Miguel Soto
 *
 * @param s a pointer to the space
 * @return a pointer to Id with all objects id that are in that spacea or NULL if there was an error
 */
Id *space_get_objects_ids(Space *s);

/**
 * @brief It gets if there is an object with id id in a space s.
 * @author Miguel Soto
 *
 * @param s a pointer to space
 * @param id of an object
 * @return TRUE if object is in space or FLASE if object is not in space. Returns FALSE if there has been an error.
 */
BOOL space_has_object(Space *s, Id id);

/**
 * @brief It alloc memory for the space_gdesc
 * @author Miguel Soto
 *
 * @return double pointer to char to the newgdesc or NULL if there has been an error
 */
char **space_create_gdesc();

/**
 * @brief It frees memory for the space_gdesc
 * @author Miguel Soto
 *
 * @param gdesc pointer to the array of pointers to the gdesc field
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_destroy_gdesc(char **gdesc);

/**
 * @brief It sets the graphic description of a space
 * @author Miguel Soto
 *
 * @param s a pointer to space
 * @param newgdesc a double char array with the graphical description
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_set_gdesc(Space *s, char **newgdesc);

/**
 * @brief It gets the graphic description from a space
 * @author Miguel Soto
 *
 * @param s a pointer to space
 * @return a double char array with the graphical description, or NULL if there was some error.
 */
char **space_get_gdesc(Space *s);

/**
 * @brief It prints the space information
 * @author Profesores PPROG
 *
 * This fucntion shows the id and name of the space, the spaces that surrounds it and wheter it has an object or not.
 * @param space a pointer to the space
 * @return OK, if everything goes well or ERROR if there was some mistake
 */
STATUS space_print(Space *space);

/**
 * @brief It returns the destination id of a given link
 * @author Nicolas Victorino
 *
 * @param l a pointer to the link
 * @return -1 if problem, if not Id of the link destination
 */
Id space_get_id_dest_by_link(Link *l);

#endif

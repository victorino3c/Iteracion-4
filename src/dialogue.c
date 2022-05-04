/** 
 * @brief It implements the dialogue interpreter module.
 * 
 * 
 * @file dialogue.c
 * @author Miguel Soto, Nicolas Victorino, Ignacio Nunez and Antonio Van-Oers
 * @version 3.0 
 * @date 27-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dialogue.h"

char matrix_command[COMMAND_SIZE][WORD_SIZE] =
{ "",                                          /*!< DC_ERROR*/
  "Exiting game...",                           /*!< DC_EXIT*/
  "You picked an object",                      /*!< DC_TAKE*/
  "You dropped an object",                     /*!< DC_DROP*/
  "You missed the attack and recieved damage", /*!< DC_ATTACK_MISSED*/
  "You hit the opponent",                      /*!< DC_ATTACK_HIT*/
  "You hit the opponent with a critical",      /*!< DC_ATTACK_CRITICAL*/
  "You moved to the *",                    /*!< DC_MOVE_N*/
  "You moved to the East",                     /*!< DC_MOVE_E*/
  "You moved to the South",                    /*!< DC_MOVE_S*/
  "You moved to the West",                     /*!< DC_MOVE_W*/
  "",                                          /*!< DC_INSPECT*/
  "Savement completed successfully",           /*!< DC_SAVE*/
  "Loading game...",                           /*!< DC_MLOAD*/
};

char matrix_event[EVENT_SIZE][WORD_SIZE] =
{ "All seems ok",                                                   /*!< DE_NOTHING*/
  "This object was not here, someone moved it...",      /*!< DE_MOVEOBJ*/
  "Be carefull! You stepped on spikes and losses 1 HP", /*!< DE_TRAP*/
  "An enemy appeared suddenly!",                        /*!< DE_SLIME*/
  "Darkness came, Night is with us.",                   /*!< DE_NIGHT*/
  "You can barely see the sun, is Day again",           /*!< DE_DAY*/
  "You get lost"                                        /*!< DE_SPAWN*/
};

struct _Dialogue
{
    char *command;
    char *event;
};

char *strfmt(char *str, char *arg) {
    char *res = (char *) malloc(strlen(str)+strlen(arg)+1);
    int k = 0;
    int i;
    for (i=0; i<strlen(str); i++) {
        if (str[i] != '*') 
            res[k++] = str[i];
        else {
            strcpy(res+k, arg);
            k += strlen(arg);
        }
    }
    res[k] = 0;
    return res;
}



Dialogue *dialogue_create()
{
    Dialogue *new_dialogue = NULL;

    new_dialogue = (Dialogue*) malloc(sizeof(Dialogue));

    /* Error control */
    if (new_dialogue == NULL)
    {
        return NULL;
    }

    new_dialogue->command = matrix_command[DC_ERROR];
    new_dialogue->event = matrix_event[DE_NOTHING];
    
    return new_dialogue;

}

STATUS dialogue_reset(Dialogue *dialogue){
    
    /*Error Control*/
    if(dialogue == NULL){
        return ERROR;
    }

    dialogue->command = matrix_command[DC_ERROR];
    dialogue->event = matrix_event[DE_NOTHING];

    return OK;
}

char *dialogue_get_command(Dialogue *dialogue)
{
    /*Error control */
    if(dialogue == NULL){
        return NULL;
    }

    return dialogue->command;
}

STATUS dialogue_set_command(Dialogue *dialogue, DC_Enum condition)
{
    /*Error control*/
    if(dialogue == NULL){
        return ERROR;
    }

    dialogue->command = matrix_command[condition];


    return OK;
}

char *dialogue_get_event(Dialogue *dialogue)
{
    /*Error control */
    if(dialogue == NULL){
        return NULL;
    }

    return dialogue->event;
}

STATUS dialogue_set_event(Dialogue *dialogue, DE_Enum condition)
{
    /*Error control*/
    if(dialogue == NULL){
        return ERROR;
    }

    dialogue->event = matrix_event[condition];

    return OK;
}

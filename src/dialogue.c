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
{ " ",                                                      /*!< DC_ERROR*/
  "Exiting game...",                                                        /*!< DC_EXIT*/   
  "You picked *",                                                           /*!< DC_TAKE*/      
  "You dropped *",                                                          /*!< DC_DROP*/      
  "You missed and * hit you . ",                                            /*!< DC_ATTACK_MISSED*/ 
  "You hit * . ",                                                           /*!< DC_ATTACK_HIT*/    
  "It was a critical hit!",                                                 /*!< DC_ATTACK_CRITICAL*/
  "You moved North, you are now in the *",                                  /*!< DC_MOVE_N*/  
  "You moved East, you are now in the *",                                   /*!< DC_MOVE_E*/   
  "You moved South, you are now in the *",                                  /*!< DC_MOVE_S*/  
  "You moved West, you are now in the *",                                   /*!< DC_MOVE_W*/ 
  "You moved Up, you are now in the *",                                     /*!< DC_MOVE_U*/     
  "You moved Down, you are now in the *",                                   /*!< DC_MOVE_D*/
  "You look around and found that the *"                                    /*!< DC_INSPECT_space*/ 
  "After a closer look, it",                                                /*!< DC_INSPECT_obj*/ 
  "Save completed successfully",                                            /*!< DC_SAVE*/
  "Loading game...",                                                        /*!< DC_MLOAD*/
  "You slept for a while, you should check the time ",                      /*!< DC_USE_BED*/
  "You used an apple, you recovered health",                                /*!< DC_USE_APPLE*/
  "You used an elixir, you recovered health",                               /*!< DC_USE_ELIXIR*/
  "You used armor, max health increased",                                   /*!< DC_USE_ARMOR*/
  "You used the hook, looks like you found a key!",                         /*!< DC_USE_HOOK*/
  "You dropped the crumbs close to your cellmate, he thanked you for that and mentioned he left the key for a hidden door in an unreachable place so that no one will ever use it",     /*!< DC_BREAD*/
  "What's with me, what am I even thinking?",                               /*!< DC_UNKNOWN*/
  "Looks like the three candles activated a mechanism, a huge shelf slides to the side, revealing the opening to a Bedroom",                                                            /*!< DC_PUZZLE*/
  "You opened a door ",                                                     /*!< DC_OPEN*/
  "How dare you attack your creator? Have your punishment, little human"    /*!< DC_HIM*/
  "So here you are, seeking a final battle. Pathetic that you believe his promises, HE will abandon you, once again, like every other time. If not His, you will suffer the Dusk wrath" /*!< DC_BOSS*/ 
};

char matrix_event[EVENT_SIZE][WORD_SIZE] =
{ "All seems ok",                                                           /*!< DE_NOTHING*/
  "This object was not here, someone moved it...",                          /*!< DE_MOVEOBJ*/
  "Be carefull! You stepped on spikes and lost 1 HP",                       /*!< DE_TRAP*/
  "A rat suddenly appeared, and it is attacking you!",                      /*!< DE_SLIME*/
  "Time seems to fly here. Darkness came and Night is upon us.",            /*!< DE_NIGHT*/
  "You can see the sun rising, it is daytime again",                        /*!< DE_DAY*/
  "You got lost and got back where you started"                             /*!< DE_SPAWN*/
};

struct _Dialogue
{
    char *command;      /*!< Command´s dialogue*/
    char *event;        /*!< Event´s dialogue*/
};

/**
 * @brief Function that substitutes the * on each default dialogue by specific modifiers
 * 
 * @param str given command string
 * @param arg Given argument
 * @return Modified command
 */
char *strmod(char *str, const char *arg);


char *strmod(char *str, const char *arg) {
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


/** Function to create a dialogue struct
*/
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

/** Function to free a given dialogue structure
*/
STATUS dialogue_destroy(Dialogue *d)
{
    /*Error control*/
    if (!d)
    return ERROR;

    free(d);
    return OK;
}

/** Resets the dialogue
*/
STATUS dialogue_reset(Dialogue *dialogue){
    
    /*Error Control*/
    if(dialogue == NULL){
        return ERROR;
    }

    dialogue->command = matrix_command[DC_ERROR];
    dialogue->event = matrix_event[DE_NOTHING];

    return OK;
}

/** Gets the dialogue information after executing a command
*/
char *dialogue_get_command(Dialogue *dialogue)
{
    /*Error control */
    if(dialogue == NULL){
        return NULL;
    }

    return dialogue->command;
}

/** Sets the dialogue information after executing a command
*/
STATUS dialogue_set_command(Dialogue *dialogue, DC_Enum condition, Space *current_loc, Object *obj, Enemy *enemy)
{
    /*Error control*/
    if(dialogue == NULL){
        return ERROR;
    }

     if (condition == DC_TAKE || condition == DC_DROP){
        if(obj == NULL)
        {
            dialogue->command = matrix_command[DC_PUZZLE];   
        }
        else
        {
            dialogue->command = strmod(matrix_command[condition], obj_get_name(obj) );
        }
         
     }

     else if (condition == DC_MOVE_N || condition == DC_MOVE_E || condition == DC_MOVE_S || condition == DC_MOVE_W || condition == DC_MOVE_U || condition == DC_MOVE_D){
        
         dialogue->command = strmod(matrix_command[condition], space_get_name(current_loc));
     }

     else if (condition == DC_MOVE_N || condition == DC_MOVE_E || condition == DC_MOVE_S || condition == DC_MOVE_W){
        
         dialogue->command = strmod(matrix_command[condition], space_get_name(current_loc));
     }

     else if (condition == DC_ATTACK_HIT || condition == DC_ATTACK_MISSED ){
        
         dialogue->command = strmod(matrix_command[condition], enemy_get_name(enemy));
     }

     else if (condition == DC_INSPECT_O){
         dialogue->command = strmod(matrix_command[condition], obj_get_description(obj));
     }

     else if (condition == DC_INSPECT_S){
         dialogue->command = strmod(matrix_command[condition], space_get_long_description(current_loc));
     }
     
     else if (condition == DC_OPEN){
         dialogue->command = strmod(matrix_command[condition], space_get_name(current_loc));
     }
     else
     {
        dialogue->command = matrix_command[condition];      
     }
   
   

    return OK;
}
/**                     
                 
    DC_OPEN,                   
                       
    **/

/** Gets the dialogue information after executing an event
*/
char *dialogue_get_event(Dialogue *dialogue)
{
    /*Error control */
    if(dialogue == NULL){
        return NULL;
    }

    return dialogue->event;
}

/** Sets the dialogue information after executing an event
*/
STATUS dialogue_set_event(Dialogue *dialogue, DE_Enum condition)
{
    /*Error control*/
    if(dialogue == NULL){
        return ERROR;
    }

    dialogue->event = matrix_event[condition];

    return OK;
}

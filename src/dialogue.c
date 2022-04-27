

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dialogue.h"

struct _Dialogue
{
    char *inspection;
    char *command;
    char *event;
};

Dialogue *dialogue_create()
{
    Dialogue *new_dialogue = NULL;

    new_dialogue = (Dialogue*) malloc(sizeof(Dialogue));

    /* Error control */
    if (new_dialogue == NULL)
    {
        return NULL;
    }

    new_dialogue->inspection = NULL;
    new_dialogue->command = NULL;
    new_dialogue->event = NULL;
    
    return new_dialogue;

}

char *dialogue_get_inspection(Dialogue *dialogue)
{
    /*Error control */
    if(dialogue == NULL){
        return NULL;
    }

    return dialogue->inspection;
}

STATUS dialogue_set_inspection(Dialogue *dialogue, char *inspection)
{
    /*Error control*/
    if(dialogue == NULL || inspection == NULL){
        return ERROR;
    }

    dialogue->inspection = inspection;

    return OK;
}
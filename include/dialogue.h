/**
 * @brief It implements the dialogue interpreter interface
 *
 * @file dialogue.h
 * @author Nicolas VIctorino, Antonio Van-Oers, Ignacio Nunez and Miguel Soto
 * @version 3.0
 * @date 27-04-2022
 * @copyright GNU Public License
 */

#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "types.h"
#include "object.h"

#define COMMAND_SIZE 14
#define EVENT_SIZE 7

typedef enum _enum_command_dialogue{
    DC_ERROR = 0,
    DC_EXIT,
    DC_TAKE,
    DC_DROP,
    DC_ATTACK_MISSED,
    DC_ATTACK_HIT,
    DC_ATTACK_CRITICAL,
    DC_MOVE_N,
    DC_MOVE_E,
    DC_MOVE_S,
    DC_MOVE_W,
    DC_INSPECT,
    DC_SAVE,
    DC_LOAD
}DC_Enum;

typedef enum _enum_event_dialogue{
    DE_NOTHING = 0,
    DE_MOVEOBJ,
    DE_TRAP,
    DE_SLIME,
    DE_NIGHT,
    DE_DAY,
    DE_SPAWN
}DE_Enum;

typedef struct _Dialogue Dialogue;

Dialogue *dialogue_create();

STATUS dialogue_reset(Dialogue *dialogue);

char *dialogue_get_command(Dialogue *dialogue);

STATUS dialogue_set_command(Dialogue *dialogue, DC_Enum condition);

char *dialogue_get_event(Dialogue *dialogue);

STATUS dialogue_set_event(Dialogue *dialogue, DE_Enum condition);

#endif

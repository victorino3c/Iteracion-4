

#ifndef DIALOGUE.H
#define DIALOGUE.H

#include "types.h"

typedef struct _Dialogue Dialogue;

Dialogue *dialogue_create();

char *dialogue_get_inspection(Dialogue *dialogue);

STATUS dialogue_set_inspection(Dialogue *dialogue, char *inspection);

#endif
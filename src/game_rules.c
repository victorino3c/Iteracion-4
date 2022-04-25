/**
 * @brief It implements features of the game
 *
 * @file game_rules.c
 * @author Miguel Soto, Nicolas Victorino, Antonio Van-Oers, Ignacio Nunez
 * @version 1.0
 * @date 25-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "../include/game_rules.h"
#include "../include/types.h"

/*Private functions*/
BOOL random(int num);



/*Returns TRUE in porcentage of num*/
BOOL random(int num){

    int rand_num;

    srand(time(NULL));
    rand_num = rand() % 100;

    if(rand_num > num){
        return FALSE;
    }

    return TRUE;
}


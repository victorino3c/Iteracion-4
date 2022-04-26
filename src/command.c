/** 
 * @brief It implements the command interpreter module.
 * 
 * It scans the cmd searching for key words introduced by the user in order to interpret and clasify the commands
 * as unknown, exit, up, down, left, right attack, take, drop, move or inspect depending on what string is typed.
 * 
 * @file command.c
 * @author Miguel Soto, Nicolas Victorino, Ignacio Nunez and Antonio Van-Oers
 * @version 3.0 
 * @date 05-04-2022
 * @copyright GNU Public License
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "../include/command.h"

#define CMD_LENGHT 30 /*!< It defines the maximun lenght from an input command */

/**
 * @brief Two dimensional string array where commands names are included. When reading commands case sensitivity is off.
 */
char *cmd_to_str[N_CMD]
[N_CMDT] = {{"", "No command"},    /*!< At the beginning, there is no commands */
      {"", "Unknown"},             /*!< If an empty or unknown string is received, it is interpreted as Unknown */
      {"e", "Exit"},               /*!< If an "e" or "exit" is received, it is interpreted as Exit */
      {"c", "Take"},               /*!< If a "c" or "take" is received, it is interpreted as Take */
      {"v", "Drop"},               /*!< If a "v" or "drop" is received, it is interpreted as Drop */
      {"q", "Attack"},             /*!< If a "q" or "attack" is received, it is interpreted as Attack */
      {"m", "Move"},               /*!< If a "m" or "move" received, it is interpreted as move */
      {"i", "Inspect"},            /*!< If an "i" or "inspect" received, it is interpreted as inspect */
      {"g", "Save"},               /*!< If an "g" or "save" received, it is interpreted as Save */
      {"l", "Load"},               /*!< If an "l" or "load" received, it is interpreted as Load */
      };
        
/**It scans the cmd searching for key words introduced by the user 
  *in order to interpret and clasify the info
  */
T_Command command_get_user_input(char *arg)
{
  T_Command cmd = NO_CMD; 
  char input[CMD_LENGHT] = "";
  int i = UNKNOWN - NO_CMD + 1;
  
  /* Error control*/
  if (!arg)
  {
    return NO_CMD;
  }
  
  if (scanf("%s", input) > 0)
  {
    cmd = UNKNOWN;
    while (cmd == UNKNOWN && i < N_CMD)     /*!< Condition to continue reading */
    {
      if (!strcasecmp(input, cmd_to_str[i][CMDS]) || !strcasecmp(input, cmd_to_str[i][CMDL]))
      {
        cmd = i + NO_CMD;                   /*!< If any differences are detected between CMDS, CMDL and the input, cmd is modified */
        if (cmd == TAKE || cmd == DROP || cmd == MOVE || cmd == INSPECT || cmd == SAVE || cmd == LOAD)
        {
          if (scanf("%s", arg) < 0)
          {
            fprintf(stdout, "Comando incorrecto. Los comandos TAKE DROP INSPECT MOVE LOAD necesitan un argumento mas como el nombre del objeto .\n");
          }
        }
        
      }
      else                                  /*!< In any other case, continue reading >! */
      {
        i++;                                 
      }
    }
  }
  
  return cmd;
}

T_Command command_get_file_input(char *command, char *arg) 
{
  T_Command cmd = NO_CMD; 
  char input[CMD_LENGHT] = "";
  int i = UNKNOWN - NO_CMD + 1, j = 0, z = 0;
  
  /* Error control*/
  if (!arg)
  {
    return NO_CMD;
  }
  
  if (command != NULL)
  {
    cmd = UNKNOWN;
    for (j = 0; command[j] != ' '; j++) { /*Reads first part of command*/
      input[j] = command[j];
    }
    input[j] = '\0';
    while (cmd == UNKNOWN && i < N_CMD)     /*!< Condition to continue reading */
    {
      if (!strcasecmp(input, cmd_to_str[i][CMDS]) || !strcasecmp(input, cmd_to_str[i][CMDL]))
      {
        cmd = i + NO_CMD;                   /*!< If any differences are detected between CMDS, CMDL and the input, cmd is modified */
        if (cmd == TAKE || cmd == DROP || cmd == MOVE || cmd == INSPECT)
        {
          for (j++, z = 0; command[j] != ' '; j++, z++) { /*Reads the second argument of command and saves it in arg*/ 
            arg[z] = command[j];
          } 
          arg[z] = '\0';
          if (arg == NULL)
          {
            fprintf(stdout, "Comando incorrecto. Los comandos TAKE DROP INSPECT MOVE necesitan un argumento mas como el nombre del objeto .\n");
          }
        }
        
      }
      else                                  /*!< In any other case, continue reading >! */
      {
        i++;                                 
      }
    }
  }
  
  return cmd;
}

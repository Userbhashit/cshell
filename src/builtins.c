#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"

struct builtins commands[] = {
  {"true", cmdTrue},
  {"false", cmdFalse},
  {"echo", cmdEcho},
};
int builtinsCmds = sizeof(commands)/sizeof(commands[0]);

void (*getBuiltinCMDFunction(char* commandName)) (char**){
  for (int i = 0; i < builtinsCmds; i++) {
    if (strcmp(commands[i].name, commandName) == 0) {
      return commands[i].fp;
    }
  }

  return NULL;
}


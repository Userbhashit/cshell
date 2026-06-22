#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "parseAndExecute.h" // To get the lastCommandExitCode

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

void cmdEcho(char** command) {
  if (command[1] && (command[1][0] == '$')) {
    if (command[1][1] == '?') {
      printf("%d\n", lastCommandExitCode);
      return;
    }

    char* env = getenv(command[1]+1);
    printf("%s", (env) ? env : " ");
  } else {
    for (int i = 1; command[i]; i++) {
      printf("%s ", command[i]);
    }
  }

  printf("\n");
}

void cmdTrue(char**) {
  exit(EXIT_SUCCESS);
}

void cmdFalse(char** command) {
  exit ((command[1]) ? atoi(command[1]) : 1);
}

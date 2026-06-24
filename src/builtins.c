#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "builtins.h"
#include "parseAndExecute.h" // To get the lastCommandExitCode

struct builtins commands[] = {
  {"true", cmdTrue, false},
  {"false", cmdFalse, false},
  {"echo", cmdEcho, false},
};
int builtinsCmdsLen = sizeof(commands)/sizeof(commands[0]);

void (*getBuiltinCMDFunction(char* commandName, bool* runInParent)) (char**){
  for (int i = 0; i < builtinsCmdsLen; i++) {
    if (strcmp(commands[i].name, commandName) == 0) {
      *runInParent = commands[i].runInParent;
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
  lastCommandExitCode = 0;
}

void cmdTrue(char**) {
  exit(EXIT_SUCCESS);
}

void cmdFalse(char** command) {
  exit ((command[1]) ? atoi(command[1]) : 1);
}

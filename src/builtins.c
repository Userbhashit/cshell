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


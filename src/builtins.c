#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "builtins.h"
#include "parseAndExecute.h" // To get the lastCommandExitCode

struct builtins commands[] = {
  {"cd", cmdCd, true},
  {"pwd", cmdPwd, true},
  {"echo", cmdEcho, false},
  {"true", cmdTrue, false},
  {"which", cmdWhich, false},
  {"false", cmdFalse, false},
};
int builtinsCmdsLen = sizeof(commands)/sizeof(commands[0]);

void (*getBuiltinCMDFunction(char* commandName, bool* runInParent)) (char**){
  for (int i = 0; i < builtinsCmdsLen; i++) {
    if (strcmp(commands[i].name, commandName) == 0) {
      if (runInParent) *runInParent = commands[i].runInParent;
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

void cmdCd(char** command) {
  int ExitCode = EXIT_SUCCESS;
  if (!command[1]) {
    fprintf(stderr, "chsell: Usage cd <path-to-directory>\n");
    ExitCode = EXIT_FAILURE;
  } else if (chdir(command[1]) < 0) {
    perror("cshell");
    ExitCode = EXIT_FAILURE;
  } 

  lastCommandExitCode = ExitCode;
}

void cmdPwd(char**) {
  int bufSize = 1024;
  char* currentDir = malloc(bufSize);
  getcwd(currentDir, bufSize);

  if (!currentDir) {
    lastCommandExitCode = EXIT_FAILURE;
    perror("pwd");
  } else {
    lastCommandExitCode = EXIT_SUCCESS;
    printf("%s\n", currentDir);
    free(currentDir);
  }
}

int searchDir(char* dirPath, char* programName) {
  DIR* dirPtr = opendir(dirPath);
  if (!dirPtr) 
    return 0;

  struct dirent* dirData = readdir(dirPtr);
  int found = 0;

  while (dirData) {
    if (strcmp(programName, dirData->d_name) == 0) {
      printf("%s/%s\n", dirPath, dirData->d_name);
      found = 1;
      break;
    }
    dirData = readdir(dirPtr);
  }

  closedir(dirPtr);
  return found;
}

void cmdWhich(char** command) {
  if (!command || !command[1])
    exit(EXIT_FAILURE);

  if (getBuiltinCMDFunction(command[1], NULL)) {
    printf("%s is a shell builtin command.\n", command[1]);
    exit(EXIT_SUCCESS);
  }

  char* pathEnvPtr = getenv("PATH");
  if (!pathEnvPtr) {
    fprintf(stderr, "Unable to get PATH variable.\n");
    exit(EXIT_FAILURE);
  }

  char* pathEnv = strdup(pathEnvPtr);

  const char* sep = ":\n";
  char* dirPath = strtok(pathEnv, sep); 

  while (dirPath) {
    if (searchDir(dirPath, command[1])) {
      exit(EXIT_SUCCESS);
    } else {
      dirPath = strtok(NULL, sep);
   }
  }

  free(pathEnv);
  exit(EXIT_FAILURE);
}


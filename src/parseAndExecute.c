#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h> 

#include "builtins.h"
#include "parseAndExecute.h"

// Helper function
static void executeInChild(char** command, void(builtinsCmdFp)(char**));

int lastCommandExitCode;

static char* expandHomePath(char* token) {
  char* newToken;
  if (token && (token[0] == '~')) {
    char* homePath = getenv("HOME");
    if (!homePath) 
      homePath = " ";

    int len = strlen(homePath) + strlen(token); // we will skip '~' so we will have extra one for '\0'
    newToken = malloc(len);

    if (!newToken) {
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
    }
    
    snprintf(newToken, len, "%s%s", homePath, token+1);
  } else {
    newToken = strdup(token);
    if (!newToken) {
      fprintf(stderr, "Unable to allocate memory.\n");
      exit(EXIT_FAILURE);
    }
  }

  return newToken;
}

char** parseQuery(char* query) {
  size_t pos = 0, bufSize = TOKEN_BUF_SIZE;
  char** tokens =  malloc(bufSize * sizeof(char*));
  if (!tokens) {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
  char* token = strtok(query, DELIMITERS);

  while (token) {
    tokens[pos++] = expandHomePath(token);

    if (pos >= bufSize) {
      bufSize += TOKEN_BUF_SIZE;
      tokens = realloc(tokens, bufSize * sizeof (char*));

      if (!tokens) {
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
      } 
    }

    token = strtok(NULL, DELIMITERS);
  }

  tokens[pos] = NULL;
  return tokens;
}

void execute(char** command) {
  if (!command || !command[0]) {
    return;
  }
  bool runInParent;
  void (*builtinCmdFp)(char**) = getBuiltinCMDFunction(command[0], &runInParent);
  (builtinCmdFp && runInParent) ? builtinCmdFp(command) : executeInChild(command, builtinCmdFp);
}

static void executeInChild(char** command, void(builtinsCmdFp)(char**)) {
  pid_t pid = fork();
  int status;

  if (pid < 0) {
    perror("Fork: ");
    return;
  }

  // child process
  if (pid == 0) {
    if (builtinsCmdFp) {
      builtinsCmdFp(command);
      exit(EXIT_SUCCESS);
    } else if (execvp(command[0], command) == -1) {
      perror("cshell");
      exit(EXIT_FAILURE);
    } 
  } else {
    // Parent process
    wait(&status);

    lastCommandExitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  }
}

void freCmd(char** command) {
  for (int i = 0; command[i]; i++) {
    free(command[i]);
  }
  free(command);
}

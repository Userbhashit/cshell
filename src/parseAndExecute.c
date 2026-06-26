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

char** parseQuery(char* query) {
  size_t pos = 0, bufSize = TOKEN_BUF_SIZE;
  char** tokens =  malloc(bufSize * sizeof(char*));
  if (!tokens) {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
  char* token = strtok(query, DELIMITERS);

  while (token) {
    tokens[pos++] = token;

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

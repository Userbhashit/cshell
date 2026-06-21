#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "parseAndExecute.h"

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
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    return;
  }

  if (pid == 0) {
    if (execvp(command[0], command) == -1) {
      perror("cshell");
      exit(EXIT_FAILURE);
    }
  } else {
    int status;
    wait(&status);

    lastCommandExitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  }
}


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define TOKEN_BUF_SIZE 8
#define DELIMITERS " \t\n\r\a"

void execute(char** command, int* exitCode);
char** parseQuery(char* query);

int exitCode = 0;

int main(void) {
  char** command;
  char* queryBuffer = NULL;
  size_t linecap = 0;
  ssize_t bufferSize;
  while (1) {
    printf("chsell> ");

    if ((bufferSize = getline(&queryBuffer, &linecap, stdin)) < 0)
      break;

    queryBuffer[bufferSize - 1] = '\0';
    
    command = parseQuery(queryBuffer);
    execute(command, &exitCode);

    free(command);
  }

  free(queryBuffer);

  return EXIT_SUCCESS;
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

void execute(char** command, int* exitCode) {
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

    if (!exitCode) return;

    *exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
  }
}


#include <stdio.h>
#include <stdlib.h>

#include "parseAndExecute.h"

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
    execute(command);

    free(command);
  }

  free(queryBuffer);

  return EXIT_SUCCESS;
}

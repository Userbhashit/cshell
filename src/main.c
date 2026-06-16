#include <stdio.h>
#include <stdlib.h>

int main(void) {
  
  char* commandBuffer = NULL;
  size_t linecap = 0;
  ssize_t commandSize;
  while (1) {
    printf("chsell> ");

    if ((commandSize = getline(&commandBuffer, &linecap, stdin)) < 0)
      break;

    commandBuffer[commandSize - 1] = '\0';
    printf("Searching and executing: %s\n", commandBuffer);
  }

  free(commandBuffer);

  return 0;
}

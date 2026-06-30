#ifndef PNE
#define PNE

#define TOKEN_BUF_SIZE 8
#define DELIMITERS " \t\n\r\a"

extern int lastCommandExitCode;

void freCmd(char** command);
void execute(char** command);
char** parseQuery(char* query);

#endif

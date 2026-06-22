#ifndef BUILTIN
#define BUILTIN

// returns the function pointer to appropriate command
void (*getBuiltinFunction(char* cpmmandName)) (char**);

// builtin commands declaration
void cmdFalse(char** command);
void cmdEcho(char** command);
void cmdTrue(char**);

struct builtins {
  char* name;
  void (*fp)(char**); 
};

extern struct builtins commands[];
extern int builtinsCmds;

#endif

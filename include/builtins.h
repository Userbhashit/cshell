#ifndef BUILTIN
#define BUILTIN

// returns the function pointer to appropriate command and where to run the process
void (*getBuiltinCMDFunction(char* commandName, bool* runInParent)) (char**);

// builtin commands declaration
void cmdFalse(char** command);
void cmdEcho(char** command);
void cmdTrue(char**);
void cmdPwd(char**);
void cmdCd(char**);

struct builtins {
  char* name;
  void (*fp)(char**); 
  bool runInParent;
};

extern struct builtins commands[];
extern int builtinsCmds;

#endif

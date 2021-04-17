#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc) {  
    
    char *system_path_commands[] = {
    "/bin/",
    NULL
};
char *cmd = "ls";
char *argv[3];
argv[0] = "ls";
argv[1] = "-la";
argv[2] = NULL;
    execvp(argv[0],argv);
  return 0;

}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(int argc, char *argv[]) {

   char *arguments[argc];
   
      for(int i = 0; i < argc; i++){
          arguments[i]=argv[i+1];
      }
    
     char *args[] = {argv[0], NULL};

  execvp(argv[1], arguments);
 
  return 0;

}
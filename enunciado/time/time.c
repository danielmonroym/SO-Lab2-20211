#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    struct timeval current_time;
    struct timeval end_process_time;
    gettimeofday(&current_time, NULL);
    int rc = fork();
  if (rc < 0) {
    // fork failed; exit
    fprintf(stderr, "fork failed\n");
    exit(1);
  } 
  else if (rc == 0) {
    char *arguments[argc];

    for (int i=1; i<argc; i++){
        arguments[i-1] = argv[i];
    }    

    arguments[argc-1] = NULL;
    execvp(arguments[0], arguments);  // run
    
  } else {
    wait(NULL);
    gettimeofday(&end_process_time, NULL);
    int sec;
    int msec;
    sec = end_process_time.tv_sec - current_time.tv_sec;
    msec= end_process_time.tv_usec - current_time.tv_usec;
    printf(" Seconds: %d Microseconds : %d \n ", sec,msec);
    }
  return 0;
}
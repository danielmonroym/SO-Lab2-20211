#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_SIZE 100

typedef enum {jason,abril, diana , endup,not_command,ls} builtin_command; 
const static struct{
    builtin_command command;
    char* string_command;
}
commands[]={
    {jason, "jason"},
    {abril, "abril"},
    {diana, "diana"},
    {endup, "exit"},
    {ls,"ls"},
};

builtin_command str_to_command(char *strcommand){
    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(strcommand, commands[i].string_command))
        {
            return commands[i].command;
        }
        
    }
    return not_command;
}

int main(int argc, char*argv[]){
    char str[MAX_SIZE];
    do
    {
        printf("wish>>");
        fgets(str, MAX_SIZE,stdin);
        char *p = str;
        while (*p != '\n')
        {
            p++;
        }
        *p = '\0';
        builtin_command command = str_to_command(str);
        if (command != not_command)
        {
            switch(command){
                case jason:
                    printf("jason executed\n");
                    break;
                case abril:
                    printf("abril executed\n");
                    break;
                case diana:
                    printf("diana executed\n");
                    break;
                case endup:
                    printf("endup executed\n");
                    break;
                default: 
                    printf("Command not found \n");
            }
        }else{

            printf("Buscar el comando en la ruta patth \n");
        }
        
        
    } while (1);
    
}
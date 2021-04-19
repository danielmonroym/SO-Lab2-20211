#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_SIZE 100
char *system_path_commands[] = {
    "./",
    "/usr/bin/",
    "/bin/",
    NULL};
typedef enum
{
    cd,
    abril,
    diana,
    endup,
    d,
    not_command,
    ls,
} builtin_command;
void parseString(char str[], char *vector[30], int numberOfFlags);
void cdWish(char **args);
void pathWish(char **args);

    const static struct
{
    builtin_command command;
    char *string_command;
} commands[] = {
    {cd, "cd"},
    {abril, "abril"},
    {diana, "diana"},
    {endup, "exit"},
    {d, "d"},
    {ls, "ls"},

};

builtin_command str_to_command(char *strcommand)
{
    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(strcommand, commands[i].string_command))
        {
            return commands[i].command;
        }
    }
    return not_command;
}

int main(int argc, char *argv[])
{
    char str[MAX_SIZE];
    do
    {
        printf("wish>>");
        fgets(str, MAX_SIZE, stdin);
        char *p = str;
        while (*p != '\n')
        {
            p++;
        }
        *p = '\0';

        char *cmd[sizeof(str)];
        parseString(str, cmd, 20);

        builtin_command command = str_to_command(str);
        if (command != not_command)
        {
            switch (command)
            {
            case cd:
                printf("cd executed\n");
                cdWish(cmd);
                break;
            case d:
                printf("path executed\n");
                pathWish(cmd);
                break;
            case diana:
                printf("diana executed\n");
                pathWish(cmd);
                break;
            case endup:
                exit(0);
                break;
            default:
                printf("Command not found \n");
            }
        }
        else
        {
            int i = 0;
            char pathToFile[MAX_SIZE];
            int returnValue = -1;

            while (system_path_commands[i] != NULL && returnValue == -1)
            {
                strcpy(pathToFile, system_path_commands[i]);
                strcat(pathToFile, cmd[0]);
                returnValue = access(pathToFile, X_OK);
                i++;
            }
            if (returnValue != -1)
            {
                printf("Melo %s \n", pathToFile);
                int rc = fork();
                if (rc < 0)
                {
                    // fork failed; exit
                    fprintf(stderr, "fork failed\n");
                    exit(1);
                }
                else if (rc == 0)
                {
                    // child (new process)
                    printf("hello, I am child (pid:%d)\n", (int)getpid());
                    execvp(cmd[0], cmd); // runs word count
                    printf("this shouldn't print out");
                }
                else
                {
                    // parent goes down this path (original process)
                    int wc = wait(NULL);
                    printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
                           rc, wc, (int)getpid());
                }
                //execvp(command[0], command);
            }
            else
            {
                printf("Unknow command\n");
            }
        }

    } while (1);
}

void cdWish(char **args)
{

    if (args[1] != NULL && args[2] == NULL)
    {
        {
            if (chdir(args[1]) == -1)
                printf("Error en la ruta especificada\n");

            char cwd[1024];

            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                fprintf(stdout, "Directorio actual: %s\n", cwd);
            }
            else
            {
                perror("getcwd() error");
            }
        }
    }
    else
    {

        printf("Error en la ruta especificada\n");
    }
};
void parseString(char str[], char *vector[30], int numberOfFlags)
{

    char *command[30];
    char *token = strtok(str, " ");
    int j = 0;

    while (token != NULL)
    {
        command[j] = token;
        token = strtok(NULL, " ");
        j++;
    }
    command[j] = NULL;
    for (int loop = 0; loop < numberOfFlags; loop++)
    {
        vector[loop] = command[loop];
    }
}

void pathWish(char **args)
{
    if (system_path_commands[0] != NULL)
        
    system_path_commands[0] = (char **)malloc(sizeof(char *));
    char *path_name = NULL;
    int index = 0;
    char **p = args;
    while (*(++p))
    {
        path_name = (char *)malloc(strlen(*p) * sizeof(char));
        stpcpy(path_name, *p);
        system_path_commands[0][index] = path_name;
        index++;
        system_path_commands[0] = (char **)realloc(system_path_commands[0], (index + 1) * sizeof(char *));
    }
    system_path_commands[0][index] = NULL;
}
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
void executer(char *cmd[]);

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
                executer(cmd);
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
                error("getcwd() error");
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
void executer(char *cmd[]){

                char *auxcmd[30];
                int index = 0;
                int auxIndex = 0;
                while (cmd[index] != NULL)
                {
                    if (!strcmp(cmd[index], "&") || cmd[index + 1] == NULL)
                    {
                        int rc = fork();
                        if (rc < 0)
                        {
                            fprintf(stderr, "fork failed\n");
                            exit(1);
                        }
                        else if (rc == 0)
                        {
                            if (cmd[index + 1] == NULL)
                            {
                                auxcmd[auxIndex] = cmd[index];
                                auxIndex++;
                            }
                            auxcmd[auxIndex] = NULL;
                            execvp(auxcmd[0], auxcmd); // runs word count
                        }
                        else
                        {
                            wait(NULL);
                            auxIndex = 0;
                        }
                    }
                    else
                    {
                        auxcmd[auxIndex] = cmd[index];
                        auxIndex++;
                    }
                    index++;
                }
}
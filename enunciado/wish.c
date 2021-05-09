#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_SIZE 100
#define BUFFER_SIZE 1024
char errorMessage[28] = "An error has occurred\n";
char *system_path_commands[30] = {
    "./",
    "/usr/bin/",
    "/bin/",
    NULL};
typedef enum
{
    cd,
    endup,
    path,
    not_command,
} builtin_command;
void parseString(char str[], char *vector[30], int numberOfFlags);
void cdWish(char **args);
void wishBatch(char *argv[]);
void printError();
void executer(char *cmd[]);
void wish();
void pathWish(char *system_path_commands[], char *cmd[]);
void wishBatch(char *argv[]);
void printError();
void parseLine(char line[], char *vector[30], int numberOfFlags);
int wish_launch_redirect(char **args, char *file);

const static struct
{
    builtin_command command;
    char *string_command;
} commands[] = {
    {cd, "cd"},
    {endup, "exit"},
    {path, "path"},
};

builtin_command str_to_command(char *strcommand)
{
    for (int i = 0; i < 3; i++)
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

    if (argc == 1)
    {

        wish();
    }
    else if (argc == 2)
    {

        wishBatch(argv);
    }
    else
    {
        printError();
    }
}

void parseLine(char line[], char *vector[30], int numberOfFlags)
{

    if (line[0] != '\0' && line[0] != '\n')
    {
        char *command[30];
        command[0] = strtok(line, " \t\n");
        int p = 0;
        while (command[p] != NULL)
        {
            p++;
            command[p] = strtok(NULL, " \n\t");
        }
        command[p + 1] = NULL;

        for (int loop = 0; loop < numberOfFlags; loop++)
        {
            vector[loop] = command[loop];
        }
    }
}
void printError()
{
    write(STDERR_FILENO, errorMessage, strlen(errorMessage));
}
void wishBatch(char *argv[])
{
    char line[BUFFER_SIZE];

    FILE *file = NULL;
    char *batchFile = strdup(argv[1]);
    file = fopen(batchFile, "r");
    if (!file)
    {
        printError();
        exit(1);
    }

    while (fgets(line, BUFFER_SIZE, file))
    {
        char *cmd[sizeof(line)];

        parseLine(line, cmd, 20);
        printf("Comando: %s\n", cmd[0]);
        builtin_command command = str_to_command(line);
        if (command != not_command)
        {
            switch (command)
            {
            case cd:
                printf("cd executed\n");
                cdWish(cmd);
                break;
            case path:
                pathWish(system_path_commands, cmd);
                int directions = 0;
                while (system_path_commands[directions] != NULL)
                {
                    printf("path: %s \n", system_path_commands[directions]);
                    directions++;
                }
                break;
            case endup:
                exit(0);
                break;
            default:
                printError();
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
                printError();
            }
        }
    }
}
void wish()
{
    char str[MAX_SIZE];
    do
    {
        printf("wish>> ");
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
                cdWish(cmd);
                break;
            case path:
                pathWish(system_path_commands, cmd);
                int directions = 0;
                while (system_path_commands[directions] != NULL)
                {
                    printf("path: %s \n", system_path_commands[directions]);
                    directions++;
                }
                break;
            case endup:
                exit(0);
                break;
            default:
                printError();
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

                if (cmd[0] != NULL)
                {
                    strcat(pathToFile, cmd[0]);
                }

                returnValue = access(pathToFile, X_OK);

                i++;
            }
            if (returnValue != -1)
            {
                int z = 0;
                int x = 0;
                int zaux = 0;
                do
                {
                    if (strcmp(cmd[z], ">") == 0)
                    {
                        x = x + 1;
                        zaux = z;
                    }
                    else if (strchr(cmd[z], '>') != NULL)
                    {
                        x = x + 2;
                    }
                    z++;
                } while (cmd[z] != NULL);
                if (x== 0)
                {
                    //no encontro ningun >
                     executer(cmd);
                }
                else if (x == 1)
                {
                    //encontro 1 >
                    if (cmd[zaux + 1] == NULL)
                    {
            
                        printError();
                    }
                    else if (cmd[zaux + 2] != NULL)
                    {
                        printError();

                    }
                    else
                    {  
                        
                        char *file = malloc(MAX_SIZE * sizeof(char *));
                        strcpy(file, cmd[zaux + 1]);
                        cmd[zaux] = NULL;
                        cmd[zaux + 1] = NULL;
                        wish_launch_redirect(cmd, file);
                    }
                }

               
            }
            else
            {
                printError();
            }
        }

    } while (1);
}

int wish_launch_redirect(char **args, char *file){
    pid_t pid;
    pid = fork();
    if (pid == 0) {
    // Child process
        int fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

        dup2(fd, 1);   // make stdout go to file
        dup2(fd, 2);   // make stderr go to file - you may choose to not do this
                   // or perhaps send stderr to another file
        close(fd);

        execvp(args[0], args);
    } else if (pid < 0) {
        // Error forking

        printError();
    } else {
        // Parent process
       wait(NULL);
    }
    return 1;
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

void executer(char *cmd[])
{

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
void pathWish(char *system_path_commands[], char *cmd[])
{
    int index = 1;
    while (cmd[index] != NULL)
    {
        int y = 0;
        int equal = 0;
        while (system_path_commands[y] != NULL)
        {
            equal = strcmp(cmd[index], system_path_commands[y]);

            if (equal == 0)
            {
                break;
            }
            if (system_path_commands[y + 1] == NULL && equal != 0)
            {
                system_path_commands[y + 1] = cmd[index];
                system_path_commands[y + 2] = NULL;
                equal = 0;
                break;
            }
            y++;
        }
        index++;
    }
}
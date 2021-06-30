
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

void execute_cmd(char *cmd)
{

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {

    }
    else
    {
        perror("getcwd() error");
    }


    strcat(cwd, cmd);
    system(cwd);
}
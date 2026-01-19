#include <stdio.h>

int main(int argc, char *argv[], char **env)
{
    int i;
    char **env_ptr;

    printf("Command line arguments:\n");

    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    printf("\nEnvironment variables:\n");

    for (env_ptr = env; *env_ptr != 0; env_ptr++)
    {
        printf("%s\n", *env_ptr);
    }
    return 0;
}
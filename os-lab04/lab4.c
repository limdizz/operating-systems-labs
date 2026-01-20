#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2]; // Для канала между ls и sort
    int pipe2[2]; // Для канала между sort и more
    pid_t pid1, pid2, pid3;

    // Создание первого канала
    if (pipe(pipe1) == -1)
    {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    // Создание второго канала
    if (pipe(pipe2) == -1)
    {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork1");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0)
    {                                  // Процесс для ls
        dup2(pipe1[1], STDOUT_FILENO); // Перенаправление stdout в pipe1
        close(pipe1[0]);               // Закрыть чтение из первого канала
        close(pipe1[1]);               // Закрыть запись в первом канале

        execlp("ls", "ls", "-la", "/dev", NULL);
        perror("execlp ls"); // Ошибка при exec
        exit(EXIT_FAILURE);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0)
    {                                  // Процесс для sort
        dup2(pipe1[0], STDIN_FILENO);  // Перенаправление stdin из pipe1
        dup2(pipe2[1], STDOUT_FILENO); // Перенаправление stdout в pipe2
        close(pipe1[1]);               // Закрыть запись в первом канале
        close(pipe1[0]);               // Закрыть чтение из первого канала
        close(pipe2[0]);               // Закрыть чтение из второго канала

        execlp("sort", "sort", NULL);
        perror("execlp sort"); // Ошибка при exec
        exit(EXIT_FAILURE);
    }

    pid3 = fork();
    if (pid3 == -1)
    {
        perror("fork3");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0)
    {                                 // Процесс для more
        dup2(pipe2[0], STDIN_FILENO); // Перенаправление stdin из pipe2
        close(pipe2[1]);              // Закрыть запись во втором канале
        close(pipe1[0]);              // Закрыть чтение из первого канала
        close(pipe1[1]);              // Закрыть запись в первом канале

        execlp("more", "more", NULL);
        perror("execlp more"); // Ошибка при exec
        exit(EXIT_FAILURE);
    }

    // Родительский процесс закрывает все каналы
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // Ожидание завершения всех дочерних процессов
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}

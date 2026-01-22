#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/my_shm"
#define SHM_SIZE 4096

int main()
{
    int shm_fd;
    int i;
    int *size;
    void *shm;
    char *data;
    const char *messages[3];

    // Подключение к разделяемой памяти
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // Подключение разделяемой памяти к адресному пространству процесса
    shm = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    size = (int *)shm;
    data = (char *)(shm + 4);

    // Запись данных и изменение размера полезных данных
    messages[0] = "AAA";
    messages[1] = "BBB";
    messages[2] = "CCC";

    for (int i = 0; i < 3; i++)
    {
        strcpy(data, messages[i]);
        *size = strlen(messages[i]);
        sleep(5);
    }

    // Установка размера полезных данных в -1
    *size = -1;

    // Отключение от разделяемой памяти
    munmap(shm, SHM_SIZE);
    close(shm_fd);

    return 0;
}

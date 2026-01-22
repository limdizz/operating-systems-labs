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
    int *size;
    void *shm;
    char *data;

    // Создание разделяемой памяти
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    // Установка размера разделяемой памяти
    if (ftruncate(shm_fd, SHM_SIZE) == -1)
    {
        perror("ftruncate");
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
    *size = 0; // Инициализация размера полезных данных

    while (1)
    {
        sleep(1); // Задержка для уменьшения нагрузки на процессор

        if (*size == -1)
        {
            // Отключение и удаление разделяемой памяти
            munmap(shm, SHM_SIZE);
            shm_unlink(SHM_NAME);
            exit(0);
        }
        else
        {
            // Вывод новых данных, если размер изменился
            if (*size > 0)
            {
                printf("New data: %.*s\n", *size, data);
            }
        }
    }

    // Закрытие дескриптора
    close(shm_fd);
    return 0;
}

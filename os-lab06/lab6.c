#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_WRITERS 5
#define NUM_READERS 5
#define ITERATIONS 5

int Data = 0;
int ReadCount = 0;

sem_t mutex; // Защищает ReadCount
sem_t wrt;   // Защищает доступ писателей

void *writer(void *arg)
{
    int id = *((int *)arg);
    free(arg);

    unsigned int seed = (unsigned int)(time(NULL) + id);

    for (int i = 0; i < ITERATIONS; i++)
    {
        int sleep_time = 1 + rand_r(&seed) % 3;
        sleep(sleep_time);

        // Попытка записи (ждем, пока освободят читатели или другой писатель)
        sem_wait(&wrt);

        // Критическая секция
        printf("[Writer %d] WRITING... Data changed from %d to %d\n", id, Data, Data + 1);
        Data++;
        sleep(1); // Время записи

        sem_post(&wrt);
        // Конец критической секции
    }

    return NULL;
}

void *reader(void *arg)
{
    int id = *((int *)arg);
    free(arg);

    unsigned int seed = (unsigned int)(time(NULL) + id + 100);

    for (int i = 0; i < ITERATIONS; i++)
    {
        int sleep_time = 1 + rand_r(&seed) % 3;
        sleep(sleep_time);

        // Входная секция читателя
        sem_wait(&mutex);
        ReadCount++;
        if (ReadCount == 1)
        {
            // Первый читатель блокирует писателей
            sem_wait(&wrt);
        }
        sem_post(&mutex);

        // Чтение (критическая секция для данных, но не для читателей)
        printf("[Reader %d] READING... Data = %d (Active Readers: %d)\n", id, Data, ReadCount);
        sleep(1);

        // Выходная секция
        sem_wait(&mutex);
        ReadCount--;
        if (ReadCount == 0)
        {
            // Последний читатель освобождает писателей
            sem_post(&wrt);
        }
        sem_post(&mutex);
    }

    return NULL;
}

int main()
{
    pthread_t writers[NUM_WRITERS];
    pthread_t readers[NUM_READERS];

    // Инициализация семафоров (0 = локальные для потоков, 1 = начальное значение)
    if (sem_init(&mutex, 0, 1) != 0)
    {
        perror("Failed to init mutex");
        exit(1);
    }
    if (sem_init(&wrt, 0, 1) != 0)
    {
        perror("Failed to init wrt");
        exit(1);
    }

    // Создание писателей
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        int *id = malloc(sizeof(int));
        if (id == NULL)
        {
            perror("Failed to allocate memory for writer ID");
            exit(1);
        }
        *id = i + 1;
        if (pthread_create(&writers[i], NULL, writer, id) != 0)
        {
            perror("Failed to create writer thread");
            exit(1);
        }
    }

    // Создание читателей
    for (int i = 0; i < NUM_READERS; i++)
    {
        int *id = malloc(sizeof(int));
        if (id == NULL)
        {
            perror("Failed to allocate memory for reader ID");
            exit(1);
        }
        *id = i + 1;
        if (pthread_create(&readers[i], NULL, reader, id) != 0)
        {
            perror("Failed to create reader thread");
            exit(1);
        }
    }

    // Ожидание завершения
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);

    // Удаление семафоров
    sem_destroy(&mutex);
    sem_destroy(&wrt);

    printf("Done. Final Data=%d\n", Data);
    return 0;
}
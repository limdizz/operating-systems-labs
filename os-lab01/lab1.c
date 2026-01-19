#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

void print_memory(const unsigned char *data, size_t size)
{
    size_t i;
    for (i = 0; i < size; i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    const char *filename;
    int fd;
    off_t file_size;
    size_t i;
    unsigned char temp;
    unsigned char *mapped;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    filename = argv[1];
    fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1)
    {
        perror("Error seeking file");
        close(fd);
        return EXIT_FAILURE;
    }

    mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED)
    {
        perror("Error mapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Memory before swapping:\n");
    print_memory(mapped, file_size);

    for (i = 0; i < file_size - 1; i += 2)
    {
        temp = mapped[i];
        mapped[i] = mapped[i + 1];
        mapped[i + 1] = temp;
    }

    printf("Memory after swapping:\n");
    print_memory(mapped, file_size);

    if (munmap(mapped, file_size) == -1)
    {
        perror("Error unmapping file");
        close(fd);
        return EXIT_FAILURE;
    }

    if (close(fd) == -1)
    {
        perror("Error closing file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
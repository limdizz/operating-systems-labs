#include <windows.h>
#include <stdio.h>

volatile int Var = 0; 

DWORD WINAPI IncrementThread(LPVOID param) {
    int* varPtr = (int*)param;
    for (int i = 0; i < 100000000; i++) {
        (*varPtr)++;
    }
    return 0;
}

DWORD WINAPI DecrementThread(LPVOID param) {
    int* varPtr = (int*)param;
    for (int i = 0; i < 100000000; i++) {
        (*varPtr)--;
    }
    return 0;
}

int main() {
    HANDLE threads[20];

    printf("Start Var: %d\n", Var);

    for (int i = 0; i < 10; i++) {
        threads[i] = CreateThread(NULL, 0, IncrementThread, (LPVOID)&Var, 0, NULL);
        if (threads[i] == NULL) {
            printf("Error: %d\n", GetLastError());
            return 1;
        }
    }

    for (int i = 10; i < 20; i++) {
        threads[i] = CreateThread(NULL, 0, DecrementThread, (LPVOID)&Var, 0, NULL);
        if (threads[i] == NULL) {
            printf("Error: %d\n", GetLastError());
            return 1;
        }
    }

    WaitForMultipleObjects(20, threads, TRUE, INFINITE);

    for (int i = 0; i < 20; i++) {
        CloseHandle(threads[i]);
    }

    printf("Final Var: %d\n", Var);

    return 0;
}
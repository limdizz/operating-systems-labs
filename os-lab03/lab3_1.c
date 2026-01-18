#include <windows.h>
#include <stdio.h>

int main() {
    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    // Инициализация структуры STARTUPINFO
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Создание процесса (Блокнот)
    if (!CreateProcess(
            "C:/Windows/System32/notepad.exe", // Путь к исполняемому файлу
            NULL, // Командная строка
            NULL, // Атрибуты процесса
            NULL, // Атрибуты потока
            FALSE, // Наследование дескрипторов
            0, // Флаги создания
            NULL, // Использовать переменные окружения
            NULL, // Использовать текущий каталог
            &si, // Указатель на структуру STARTUPINFO
            &pi) // Указатель на структуру PROCESS_INFORMATION
       ) {
        printf("Ошибка при создании процесса: %d\n", GetLastError());
        return 1;
    }

    // Ожидание завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Закрытие дескрипторов
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
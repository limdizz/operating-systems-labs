Лабораторная работа №3. Процессы, потоки и операции над ними (задание 1).

Постановка задачи
-----------------

Требуется создать 2 программы под ОС Windows.

Программа 1 должна выполнять следующие действия:
1. Создание процесса-потомка, в котором выполняется программа "Notepad(Блокнот)".
2. Ожидание завершения процесса-потомка и отключение от неиспользуемых объектов ядра.

Программа 2 должна выполнять следующие действия:
1. Вывод начального значения целочисленной переменной Var, объявленной с квалификатором volatile.
2. Запуск 20 потоков, каждый из которых выполняет 100 000 000 изменений переменной Var,
   адрес которой должен быть передан ему в качестве входного параметра.
   При этом 10 потоков 100 000 000 раз увеличивают переменную Var на 1,
   10 потоков 100 000 000 раз уменьшают переменную Var на 1.
3. Вывод конечного значения целочисленной переменной Var.

Дополнительные требования
-------------------------

1. В программах должны использоваться вызовы функций CreateProcess(), CreateThread(), 
WaitForSingleObject() (или WaitForMultipleObjects()), CloseHandle(), ExitThread() (последняя - не обязательно).
2. Вызовы функций должны сопровождаться проверкой возвращаемых значений.
3. Запустите вторую программу несколько раз. Сравните возвращаемые значения.

Дополнения и замечания
----------------------
1. Лабораторная работа размещена по адресу \\S-FILE2\globaltemp\VMWare\OS-Labs\Lab03
2. Для создания программ рекомендуется использовать среду разработки Visual Studio.
3. Ссылки на описания функций в MSDN
 - https://docs.microsoft.com/ru-ru/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessa
 - https://docs.microsoft.com/ru-ru/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
 - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
 - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
 - https://docs.microsoft.com/windows/desktop/api/handleapi/nf-handleapi-closehandle

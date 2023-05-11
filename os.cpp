#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

// Запуск ОС, инициализация основных элемнтов системы,
// активация начальной задачи.
int StartOS(TTaskCall entry, int priority, char *name) {
    // Начальные настройки системы.
    printf("StartOS!\n");
    RunningTask = _NULL;
    HeadTask = _NULL;
    FreeTask = 0;

    // Инициализация списка задач.
    for (int i = 0; i < MAX_TASK; i++) {
        TaskQueue[i].ref = i + 1; // Связываем задачи в список.
        TaskQueue[i].state = TASK_SUSPENDED; // Пустая ячейка = SUSPENDED.
    }

    TaskQueue[MAX_TASK - 1].ref = _NULL; // Последняя зачада указывает в никуда.
    ActivateTask(entry, priority, name); // Активация начальной задачи.

    return 0;
}

// Завершение работы ОС.
void ShutdownOS() {
    printf("ShutdownOS!\n");
}


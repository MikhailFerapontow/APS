#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

// Инициализация задачи в системе.
void ActivateTask(TTaskCall entry, int priority, char *name) {
    printf("[ActivateTask] %s\n", name);

    int occupy = FreeTask; // Получаем номер задачи в списке.
    FreeTask = TaskQueue[occupy].ref; // Смещение первой свободной ячейки.

    // Инициализируем поля задачи.
    TaskQueue[occupy].priority = priority;
    TaskQueue[occupy].name = name;
    TaskQueue[occupy].waiting_events = 0;
    TaskQueue[occupy].entry = entry;
    TaskQueue[occupy].switchNumber = 0;
    TaskQueue[occupy].res = _NULL;
    TaskQueue[occupy].state = TASK_READY;

    printf("End of Activate %s\n", name);

    Schedule(occupy); // Вызываем планировщик для этой задачи.
}

// Завершение задачи.
void TerminateTask(void) {
    int task = RunningTask; // Номер текущей выполняющейся задачи.

    printf("[Terminate Task] %s\n", TaskQueue[task].name);
    TaskQueue[task].state = TASK_DONE; // Говорим, что задача выполнена.
    RunningTask = _NULL; // Теперь ничего не выполняется.

    printf("End of Terminate Task %s\n", TaskQueue[task].name);

    Dispatch(); // После завершения задачи возьмем следующую на выполнение.
}

// Планировщик задач.
void Schedule(int task, int mode) {
    printf("[Schedule] %s\n", TaskQueue[task].name);

    int cur = HeadTask; // Текущая задача - самая первая в списке.
    int prev = _NULL; // Предыдущей пока нет.
    int priority = TaskQueue[task].priority; // Получаем приоритет текущей.

    // Реализация preemptive алгоритма планирования.
    // Проходимся по списку.
    // Пока приортет задач в списке больше, чем приоритет нашей задачи...
    while (cur != _NULL && TaskQueue[cur].priority > priority) {
        // Переходим к следующей задачи в списке.
        prev = cur;
        cur = TaskQueue[cur].ref;
    }

    // Доходим до последней задачи в списке с таким же приоритетом,
    // как и у нашей задачи.
    if (mode == INSERT_TO_TAIL) {
        while (cur != _NULL && TaskQueue[cur].priority == priority) {
            prev = cur;
            cur = TaskQueue[cur].ref;
        }
    }

    // Сейчас в cur у нас указатель первую задачу, у которой приоритет
    // будет ниже, чем у новой задачи.

    TaskQueue[task].ref = cur; // Новая задача будет указывать на следующую.
    if (prev == _NULL) { HeadTask = task; } // Мы добавили первую задачу.
    else TaskQueue[prev].ref = task; // А предыдущая задача указет на нову.
    TaskQueue[task].state = TASK_READY; // Новая задача готова к выполнению.

    printf("End of Schedule %s\n", TaskQueue[task].name);

    Dispatch(); // Вызываем диспетчер.
}

// Диспетчеризация задач, постановка на выполнение.
void Dispatch() {
    int task = HeadTask; // Указатель на голову списка задач.
    // Проходимся по всему списку задач.
    while (task != _NULL) {
        // В зависимости от состояния текущей задачи с высшим приоритетом...
        switch (TaskQueue[task].state) {
            case TASK_DONE: // Если очередная задача уже выполнена...
                // Просто переходим к следующей.
                task = TaskQueue[task].ref;
                continue;
            case TASK_RUNNING: // Если очередная задача выполняется...
                // Значит она продолжит выполняться.
                printf("%s is running\n", TaskQueue[task].name);
                return;
            case TASK_READY: // Если очередная задача готова к выполнению...
                if (RunningTask != _NULL) { // Если что-то выполняется...
                    task = TaskQueue[task].ref;
                    continue;
                }
                // Запускаем задачу на выполнени.
                printf("%s started\n", TaskQueue[task].name);
                RunningTask = task;
                TaskQueue[task].state = TASK_RUNNING;

                // Даем задаче ресурс.
                if (TaskQueue[task].res != _NULL) {
                    PIP_GetRes(TaskQueue[task].res);
                }
                TaskQueue[task].switchNumber++;
                // Если первый раз включили задачу...
                if (TaskQueue[task].switchNumber == 1) {
                    TaskQueue[task].entry(); // Просто начинаем выполнять.
                } else { // Иначе...
                    // Восстанавливаем контекст.
                    longjmp(TaskQueue[task].context, 1);
                }
                return;
            case TASK_WAITING:
                // Если задача обладает всеми событиями, которые ожидает...
                task = TaskQueue[task].ref;
                continue;
         }
    }
}


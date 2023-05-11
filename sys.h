
#ifndef APS_OS_REALTIME_SYS_H
#define APS_OS_REALTIME_SYS_H

#include "defs.h"
#include <csetjmp>
#include "string.h"

// Режимы работы планировщика.
#define INSERT_TO_TAIL 1 // Вставка в конец.
#define INSERT_TO_HEAD 0 // Вставка в начало.

#define _NULL -1 // Указатель в никуда.

// Возможные состояния задачи.
enum TTaskState {
    TASK_RUNNING,
    TASK_READY,
    TASK_SUSPENDED,
    TASK_WAITING,
    TASK_DONE
};

typedef int TEventMask;
extern TEventMask WorkingEvents;

// Задача.
typedef struct Type_Task {
    int ref; // Ссылка на следующую задачу.
    int priority; // RMA планирование.
    TTaskState state; // Состояние задачи.
    TEventMask waiting_events;

    int res;
    unsigned int switchNumber = 0; // Количество переключений на эту задачу.

    void (*entry)(void); // Указатель на функцию.

    jmp_buf context;
    char *name;
} TTask;

typedef struct Type_resource {
    int task;
    int priority;
    const char *name;
} TResource;

extern int HeadTask;

extern TTask TaskQueue[MAX_TASK]; // Массив задач.
typedef struct Event {
    Event(char *name, TEventMask mask) {
        this->task = name;
        this->mask = mask;
    }

    char *task;
    TEventMask mask;
} TEvent;

// Объявление глобальных переменных.
extern TResource ResourceQueue[MAX_RES];
extern TResource EventQueue[MAX_EVENTS];
extern int RunningTask; // Номер текущей выполняющейся задачи.
extern int FreeTask;
extern int FreeResource;

void Schedule(int task, int mode = INSERT_TO_TAIL);

void Dispatch();


#endif //APS_OS_REALTIME_SYS_H

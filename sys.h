
#ifndef APS_OS_REALTIME_SYS_H
#define APS_OS_REALTIME_SYS_H

#include "defs.h"
#include <csetjmp>
#include "string.h"

#define INSERT_TO_TAIL 1
#define INSERT_TO_HEAD 0

#define _NULL -1

enum TTaskState {
    TASK_RUNNING,
    TASK_READY,
    TASK_SUSPENDED,
    TASK_WAITING,
    TASK_DONE
};

typedef int TEventMask;
extern TEventMask WorkingEvents;

typedef struct Type_Task {
    int ref;
    int priority;
    TTaskState state;
    TEventMask waiting_events;

    int res;
    unsigned int switchNumber = 0;

    void (*entry)(void);

    jmp_buf context;
    char *name;
} TTask;

typedef struct Type_resource {
    int task;
    int priority;
    const char *name;
} TResource;

extern int HeadTask;

extern TTask TaskQueue[MAX_TASK];
typedef struct Event {
    Event(char *name, TEventMask mask) {
        this->task = name;
        this->mask = mask;
    }

    char *task;
    TEventMask mask;
} TEvent;

extern TResource ResourceQueue[MAX_RES];
extern TResource EventQueue[MAX_EVENTS];
extern int RunningTask;
extern int FreeTask;
extern int FreeResource;

void Schedule(int task, int mode = INSERT_TO_TAIL);

void Dispatch();


#endif

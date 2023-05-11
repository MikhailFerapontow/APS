#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>
#include "string.h"
#include <stdexcept>

void SetSysEvent(TEventMask mask) {
    printf("SetSysEvent %i\n", mask);
    WorkingEvents |= mask;
    for (int i = 0; i < MAX_TASK; i++) {
        if (TaskQueue[i].state == TASK_WAITING &&
            (WorkingEvents & TaskQueue[i].waiting_events)) {
            TaskQueue[i].waiting_events &= !mask;
            TaskQueue[i].state = TASK_READY;
            printf("Task \"%s\" is ready\n", TaskQueue[i].name);
        }
    }
    WorkingEvents &= !mask;
    Dispatch();
    printf("End of SetSysEvent %i\n", mask);
}

void GetSysEvent(TEventMask *mask) {
    *mask = WorkingEvents;
}

void WaitSysEvent(TEventMask mask) {
    printf("WaitSysEvent %i\n", mask);
    TaskQueue[RunningTask].waiting_events = mask;
    if ((WorkingEvents & mask) == 0) {
        TaskQueue[RunningTask].state = TASK_WAITING;

        int task = RunningTask;
        RunningTask = _NULL;
        setjmp(TaskQueue[task].context);

        Dispatch();
    }
    printf("End of WaitSysEvent %i\n", mask);
}


#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>
#include <stdexcept>

// Инициализация ресурса в системе.
int InitRes(const char *name) {

    int resource = FreeResource;
    FreeResource++;
    ResourceQueue[resource].name = name;
    ResourceQueue[resource].priority = 0;
    ResourceQueue[resource].task = _NULL;
    return resource;
}


void PIP_GetRes(int res) {
    printf("GetResource %s\n", ResourceQueue[res].name);

    if (TaskQueue[RunningTask].priority < ResourceQueue[res].priority) {
        printf("%s is trying to capture the res of higher priority",
               TaskQueue[RunningTask].name);
        throw std::exception();
    }
    if (ResourceQueue[res].task != _NULL) {
        TaskQueue[ResourceQueue[res].task].state = TASK_WAITING;
    }

    TaskQueue[RunningTask].res = res;
    ResourceQueue[res].priority = TaskQueue[RunningTask].priority;
    ResourceQueue[res].task = RunningTask;

    printf("Resource %s captured by %s\n", ResourceQueue[res].name,
           TaskQueue[RunningTask].name);
}

void PIP_ReleaseRes(int res) {
    printf("Release resource %s\n", ResourceQueue[res].name);

    if (RunningTask != ResourceQueue[res].task) {
        printf("%s is trying to release the resource %s, which "
               "is not captured by it", TaskQueue[RunningTask].name,
               ResourceQueue[res].name);
        throw std::exception();
    }

    TaskQueue[ResourceQueue[res].task].res = _NULL;
    ResourceQueue[res].task = _NULL;
    ResourceQueue[res].priority = 0;

    printf("Resource %s released by %s\n", ResourceQueue[res].name,
           TaskQueue[RunningTask].name);
    Dispatch();
}

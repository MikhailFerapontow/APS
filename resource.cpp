#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>
#include <stdexcept>

// Инициализация ресурса в системе.
int InitRes(const char *name) {
    // Инициализируем ресурс в системе
    int resource = FreeResource; // Берем первую свободную ячейку ресурсов.
    FreeResource++;
    ResourceQueue[resource].name = name;
    ResourceQueue[resource].priority = 0; // Изначально приоритет 0.
    ResourceQueue[resource].task = _NULL; // Пока никем не занят.
    return resource;
}


// Захват ресурса выполняющейся задачей.
// PIP (Priority Inheritance Protocol) (протокол наследования приоритетов).
void PIP_GetRes(int res) {
    printf("GetResource %s\n", ResourceQueue[res].name);

    // PIP алгоритм не предполагает захват ресурса задачей меньшего приоритета.
    // Если выполяющаяся таска имеет меньший приоритет, чем ресурс...
    if (TaskQueue[RunningTask].priority < ResourceQueue[res].priority) {
        // Выбрасываем соответствующее исключение.
        printf("%s is trying to capture the res of higher priority",
               TaskQueue[RunningTask].name);
        throw std::exception();
    }
    // Если ресурс занят.
    if (ResourceQueue[res].task != _NULL) {
        // Переводим таску, которая заняла этот ресурс в ожидание.
        TaskQueue[ResourceQueue[res].task].state = TASK_WAITING;
    }

    // Даем ресурс выполняющейся задаче.
    TaskQueue[RunningTask].res = res;
    // Даем ресурсу приоритет выполняющейся задачи (которая взяла ресурс).
    ResourceQueue[res].priority = TaskQueue[RunningTask].priority;
    // Даем ресурсу выполняющуюся задачу.
    ResourceQueue[res].task = RunningTask;

    printf("Resource %s captured by %s\n", ResourceQueue[res].name,
           TaskQueue[RunningTask].name);
}

void PIP_ReleaseRes(int res) {
    printf("Release resource %s\n", ResourceQueue[res].name);

    // Если таска ресурса не была той, которая выполняется.
    if (RunningTask != ResourceQueue[res].task) {
        printf("%s is trying to release the resource %s, which "
               "is not captured by it", TaskQueue[RunningTask].name,
               ResourceQueue[res].name);
        throw std::exception();
    }

    // Забираем ресурс от задачи.
    TaskQueue[ResourceQueue[res].task].res = _NULL;
    ResourceQueue[res].task = _NULL; // Забираем задачу от ресурса.
    ResourceQueue[res].priority = 0; // Обнуляем приоритет ресурса.

    printf("Resource %s released by %s\n", ResourceQueue[res].name,

           TaskQueue[RunningTask].name);

    Dispatch();
}

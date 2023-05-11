#include <stdio.h>
#include "rtos_api.h"

// Объявление задач.
DeclareTask(Task1, 1);
DeclareTask(Task2, 2);
DeclareTask(Task3, 3);
DeclareTask(Task4, 1);
DeclareTask(Task5, 2);
DeclareTask(Task6, 2);
DeclareTask(Task7, 0);
DeclareTask(Task8, 1);
DeclareTask(Task9, 2);
DeclareTask(Task10, 3);


char nameTask6[] = "Task6";
char nameTask8[] = "Task8";
char nameTask9[] = "Task9";

// Объявления событий.

DeclareSysEvent(1);
DeclareSysEvent(2);
DeclareSysEvent(3);

int Res1;

int main(void) {
    Res1 = InitRes("Res1");
    char name1[] = "Task1";
    char name2[] = "Task4";
    char name3[] = "Task6";
    char name4[] = "Task8";
    //StartOS(Task1, Task1prior, name1);
    //StartOS(Task4, Task4prior, name2);
    StartOS(Task6, Task6prior, name3);
    // StartOS(Task8, Task8prior, name4);

    ShutdownOS();
    return 0;
}

// Test 1. preemptive algorithm
TASK(Task1) {
    printf("\nThis is Task1!\n\n");
    char name[] = "Task2";
    char name1[] = "Task3";
    ActivateTask(Task2, Task2prior, name);
    ActivateTask(Task3, Task3prior, name1);
    TerminateTask();
}

TASK(Task2) {
    printf("\nThis is Task2!\n\n");
    TerminateTask();
}

TASK(Task3) {
    printf("\nThis is Task3!\n\n");
    TerminateTask();
}
// Test 2. PIP resource managment algorithm
TASK(Task4) {
    printf("\nThis is Task4!\n\n");
    PIP_GetRes(Res1);
    char name[] = "Task5";
    ActivateTask(Task5, Task5prior, name);
    PIP_ReleaseRes(Res1);
    TerminateTask();
}

TASK(Task5) {
    printf("\nThis is Task5!\n\n");
    PIP_GetRes(Res1);
    PIP_ReleaseRes(Res1);
    TerminateTask();
}
// Test 3. Tasks events
TASK(Task6) {
    printf("\nThis is Task6!\n\n");
    char name[] = "Task7";
    ActivateTask(Task7, Task7prior, name);
    WaitSysEvent(Event_1);
    TEventMask event;
    GetSysEvent(&event);
    printf("GetEvent=%i\n", event);
    printf("Task6\n");
    TerminateTask();
}

TASK(Task7) {
    printf("\nThis is Task7!\n\n");
    SetSysEvent(Event_1);
    printf("Task7\n");
    TerminateTask();
}

// TASK(Task6) {
//     printf("\nThis is Task6!\n\n");
//     char name[] = "Task7";
//     ActivateTask(Task7, Task7prior, name);
//     WaitEvent(Event1);
//     printf("\nTask6 finished\n\n");
//     TerminateTask();
// }

// TASK(Task7) {
//     printf("\nThis is Task7!\n\n");
//     char name[] = "Task6";
//     SetEvent(name, Event1);
//     printf("\nTask7 finished\n\n");
//     TerminateTask();
// }
// // Test 4. All functionality
// TASK(Task8) {
//     printf("\nStart Task8\n\n");
//     char name[] = "Task9";
//     ActivateTask(Task9, Task9prior, name);
//     WaitEvent(Event2);
//     printf("\nTask 8 finished\n\n");
//     TerminateTask();
// }

// TASK(Task9) {
//     printf("\nStart Task9\n\n");
//     PIP_GetRes(Res1);
//     char name1[] = "Task10";
//     ActivateTask(Task10, Task10prior, name1);
//     char name2[] = "Task8";
//     SetEvent(name2, Event2);
//     WaitEvent(Event3);
//     PIP_ReleaseRes(Res1);
//     printf("\nTask 9 finished\n\n");
//     TerminateTask();
// }

// TASK(Task10) {
//     printf("\nStart Task10\n\n");
//     PIP_GetRes(Res1);
//     char name[] = "Task9";
//     SetEvent(name, Event3);
//     PIP_ReleaseRes(Res1);
//     printf("\nTask 10 finished\n\n");
//     TerminateTask();
// }


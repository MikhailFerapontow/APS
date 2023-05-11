#include "sys.h"

TTask TaskQueue[MAX_TASK];
TResource ResourceQueue[MAX_RES];
TResource EventQueue[MAX_EVENTS];

int RunningTask;
int FreeTask = 0; // Указатель на свободную ячейку.
int HeadTask = 0; // Указатель на голову списка задач.
int FreeResource = 0;
TEventMask WorkingEvents;

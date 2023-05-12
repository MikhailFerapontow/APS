#include "sys.h"

TTask TaskQueue[MAX_TASK];
TResource ResourceQueue[MAX_RES];
TResource EventQueue[MAX_EVENTS];

int RunningTask;
int FreeTask = 0;
int HeadTask = 0;
int FreeResource = 0;
TEventMask WorkingEvents;

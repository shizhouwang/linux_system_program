#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H

#include "tools.h"

extern Boolean bsUseSemUndo;    /* Use SEM_UNDO during semop() */
extern Boolean bsRetryOnEintr;  /* Retry is semop() interrupted by signal handler? */

int initSemAvailible(int semId, int semNum);
int initSemInUse(int semId, int semNum);
int reserveSem(int semId, int semNum);
int releaseSem(int semId, int semNum);

#endif

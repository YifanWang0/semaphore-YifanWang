#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define SHKEY 24601
#define SEMKEY 24602
#define SIZE 1000

union semun {
    int val;		        // Value for SETVAL
	struct semid_ds *buf;   //Buffer for IPC_STAT, IPC_SET
	unsigned short *array;	//Array for GETALL, SETALL
	struct seminfo *__buf;	//Buffer for IPC_INFO (Linux-specific)
};

void run();
int creating();
int removing();
int viewing();
int runWrite();

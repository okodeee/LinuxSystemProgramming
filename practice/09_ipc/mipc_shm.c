/*
 * mipc_shm.c
 * mipc.c를 shared memory system call을 사용하도록 변경
 * parent와 child가 번갈아가며 shared memory 값 1 증가
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


int
update(long *ptr)
{
	return ((*ptr)++);
}

main()
{
	int		fd, i, counter;
	pid_t	pid;
	int shmid;
	char *ptr, *shmptr;

	if((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE))<0){//make shm
		perror("shmget");
		exit(1);
	}	

	if((shmptr = shmat(shmid, 0, 0))==(void *) -1){//get pointer of shm
		perror("shmat");
		exit(1);
	}

	TELL_WAIT();
	if ((pid = fork()) < 0)  {//fork
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			if ((counter = update((long *) shmptr)) != i)  {//get parent for even num
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD();
			WAIT_CHILD();
		}
	}
	else  {
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT();
			if ((counter = update((long *) shmptr)) != i)  {//get child for odd num
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
		
		// physical memory 삭제
		if (shmctl(shmid, IPC_RMID, 0) < 0)  {
			perror("shmctl");
			exit(1);
		}
	}
}

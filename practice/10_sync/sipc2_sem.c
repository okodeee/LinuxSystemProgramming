/*
 * sipc2_sem.c
 *
 * IPC 실습내용 중 sipc2.c를 다음과 같이 수정
 * 1. busy waiting을 없애고, semaphore를 통해 동기화시킴
 * 2. buisy waiting을 위해 사용했던 flag 4 bytes를 제외하고
 *  shared memory 부분은 모두 데이터 전달을 위해 사용
 *
 * semlib.h 사용 (POSIX semaphore-like)
 *
 * sipc1_sem을 먼저 실행, sipc1_sem의 답장을 받으면
 * semaphore와 shared memory 삭제
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sipc.h"
#include "semlib.h"


main()
{
    int     shmid;
    char    *ptr;
	int     emptySemid, fullSemid, mutexSemid;

	// 새로 만들거나 기존의 segment 열기
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }
    if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }
    // semaphore 생성
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }


	if (semWait(emptySemid) < 0)  { // = wait(empty)
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    if (semWait(mutexSemid) < 0)  { // = wait(mutex)
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    sprintf(ptr, "This is a request from %d.", getpid());
    printf("Sent a request.....\n");

    if (semPost(mutexSemid) < 0)  { // = signal(mutex)
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }
    if (semPost(fullSemid) < 0)  {  // = signal(full)
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }

	
	sleep(2); 	//상대측에서 사용할 수 있도록 텀을 둠
	

	if (semWait(fullSemid) < 0)  {  // = wait(full)
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }
    if (semWait(mutexSemid) < 0)  { // = wait(mutex)
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    printf("Received reply: %s\n", ptr);


    // sipc2에서 semaphore destroy
    if (semDestroy(emptySemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(fullSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(mutexSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    // physical memory 삭제
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }
}

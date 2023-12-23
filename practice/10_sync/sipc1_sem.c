/*
 * sipc1_sem.c
 *
 * IPC 실습내용 중 sipc1.c를 다음과 같이 수정
 * 1. busy waiting을 없애고, semaphore를 통해 동기화시킴
 * 2. buisy waiting을 위해 사용했던 flag 4 bytes를 제외하고
 *	shared memory 부분은 모두 데이터 전달을 위해 사용
 *
 * semlib.h 사용 (POSIX semaphore-like)
 *
 * sipc1_sem을 먼저 실행, sipc2_sem의 메세지를 기다림
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
    // semaphore initialize, sipc1에서 초기화
	if (semInitValue(emptySemid, 1) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(fullSemid, 0) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(mutexSemid, 1) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }


	if (semWait(fullSemid) < 0)  {  // = wait(full)
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }
    if (semWait(mutexSemid) < 0)  { // = wait(mutex)
        fprintf(stderr, "semWait failure\n");
        exit(1);
    }
    printf("Received request: %s.....\n", ptr);
    sprintf(ptr, "This is a reply from %d.", getpid());
    printf("Replied.\n");

    if (semPost(mutexSemid) < 0)  { // = signal(mutex)
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }
	if (semPost(fullSemid) < 0)  { // = signal(full)
        fprintf(stderr, "semPost failure\n");
        exit(1);
    }
}
        

// POSIX semaphore-like library using System V semaphore
#include <stdio.h>
#include "semlib.h"
 
 
int
semInit(key_t key)	// Obtain a semaphore set ID
{
    int     semid;

    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
		perror("semget");
        return -1;
    }

    return semid;
}
 
int
semInitValue(int semid, int value)	// 초기값 설정
{
    union semun {
            int     val;
    } semun;

    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
		perror("semctl");
        return -1;
    }
 
    return semid;
}
 
int
semWait(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;			// 하나만 사용하기 때문에
    semcmd.sem_op = -1;			// wait
    semcmd.sem_flg = SEM_UNDO;	// 프로세스 비정상적으로 종료 시 가지고 있던 semaphore 반납
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}
 
int
semTryWait(int semid) // 기다리지 않고 fail return
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semPost(int semid)
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1;		// post
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semGetValue(int semid)		// 현재 값 가져오기
{
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy);
}
 
int
semDestroy(int semid)	// semaphore 삭제
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
		perror("semctl");
        return -1;
    }
    close(semid);
 
    return 0;
}

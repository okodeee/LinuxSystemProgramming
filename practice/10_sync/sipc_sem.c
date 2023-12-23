/*
 * sipc_sem.c
 * sipc1, sipc2의 multi-threads ver.
 * shared memory를 없애고, 전역변수로 설정
 * using semaphore
 * IPC2가 먼저 메세지를 보내고 IPC1에서 답장하도록 구현
 *==================================================
 * semaphore보다 mutex & cond가 더 직관적이다.
 * mutex는 용도가 고정되어 있어서 읽기 편하다.
 * mutex & cond는 mutex lock을 먼저 하고 cond 처리
 * mutex & cond는 초기화가 필요없다.
 * mutex는 binary semaphore와 동일하다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "sipc.h"

char				Data[1024];	// 전역변수 = shared data
sem_t				RequestSem, ReplySem, MutexSem;

void
ThreadUsleep(int usecs)
{
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;
    struct timespec     ts;
    struct timeval      tv;

    if (pthread_cond_init(&cond, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }
    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + usecs/1000000;
    ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
    if (ts.tv_nsec >= 1000000000)  {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    if (pthread_mutex_lock(&mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    if (pthread_cond_destroy(&cond) < 0)  {
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }
    if (pthread_mutex_destroy(&mutex) < 0)  {
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

void
IPC1(void *dummy)
{
	printf("IPC1: Start..... tid: %ld\n", pthread_self());

	if (sem_wait(&RequestSem) < 0)  {	// = wait(request)
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if (sem_wait(&MutexSem) < 0)  { // = wait(mutex)
		perror("sem_wait");
		pthread_exit(NULL);
	}
    printf("IPC1: Received request: %s.....\n", Data);
    sprintf(Data, "This is a reply from %ld.", pthread_self());
    printf("IPC1: Replied.\n");

	if (sem_post(&MutexSem) < 0)  {	// = signal(mutex)
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&ReplySem) < 0)  {	// = signal(reply)
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void
IPC2(void *dummy)
{
	printf("IPC2: Start..... tid: %ld\n", pthread_self());

	if (sem_wait(&MutexSem) < 0)  {	// = wait(mutex)
		perror("sem_wait");
		pthread_exit(NULL);
	}
   	sprintf(Data, "This is a reply from %ld.", pthread_self());
    printf("IPC2: Sent a request.....\n");

	if (sem_post(&MutexSem) < 0)  {	// = signal(mutex)
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&RequestSem) < 0)  {	// = signal(request)
		perror("sem_post");
		pthread_exit(NULL);
	}
		
	

    if (sem_wait(&ReplySem) < 0)  {  // = wait(reply)
        perror("sem_wait");
        pthread_exit(NULL);
    }
    if (sem_wait(&MutexSem) < 0)  { // = wait(mutex)
        perror("sem_wait");
        pthread_exit(NULL);
    }
    printf("IPC2: Received request: %s.....\n", Data);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

	// semaphore 생성 및 초기화
	if (sem_init(&RequestSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&ReplySem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	// Create thread
	if (pthread_create(&tid1, NULL, (void *)IPC1, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)IPC2, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}


	// semaphore 삭제
	if (sem_destroy(&RequestSem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&ReplySem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}
}

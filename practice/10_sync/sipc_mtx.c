/*
 * sipc_mtx.c 
 * sipc1, sipc2의 multi-threads ver.
 * shared memory를 없애고, 전역변수로 설정
 * using mutex & condition variable
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
#include <pthread.h>
#include <sys/time.h>

int					count = 0;
char				Data[1024];	// 전역변수 = shared data
pthread_cond_t      Request; // IPC2 -> IPC1 request
pthread_cond_t      Reply; // IPC1 -> IPC2 reply
pthread_mutex_t     Mutex;

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

    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if (pthread_cond_wait(&Request, &Mutex) < 0)  { // IPC2의 request 기다림
        perror("pthread_cond_wait");
        pthread_exit(NULL);
	}
	// IPC2의 메세지를 받고 답장하기
    printf("IPC1: Received request: %s.....\n", Data);
    sprintf(Data, "This is a reply from %ld.", pthread_self());
    printf("IPC1: Replied.\n");

    if (pthread_cond_signal(&Reply) < 0)  { //
        perror("pthread_cond_signal");
        pthread_exit(NULL);
    }
    if (pthread_mutex_unlock(&Mutex) < 0)  { //
        perror("pthread_mutex_unlock");
        pthread_exit(NULL);
    }

	pthread_exit(NULL);
}

void
IPC2(void *dummy)
{
	printf("IPC2: Start..... tid: %ld\n", pthread_self());

    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

   	sprintf(Data, "This is a reply from %ld.", pthread_self());
    printf("IPC2: Sent a request.....\n");
	count++;

    if (pthread_cond_signal(&Request) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if (pthread_mutex_unlock(&Mutex) < 0)  {
        perror("pthread_cond_wait");
        pthread_exit(NULL);
    }
		
	

    if (pthread_mutex_lock(&Mutex) < 0)  {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }

    if (pthread_cond_wait(&Reply, &Mutex) < 0)  { // IPC1의 reply 기다림
        perror("pthread_cond_wait");
        pthread_exit(NULL);
	}
    printf("IPC2: Received request: %s.....\n", Data);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

    // condition_variable 생성
    if (pthread_cond_init(&Request, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_cond_init(&Reply, NULL) < 0)  {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    // mutex 생성, 초기값 unlock
    if (pthread_mutex_init(&Mutex, NULL) < 0)  {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
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

    if (pthread_cond_destroy(&Request) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_cond_destroy(&Reply) < 0)  {
        perror("pthread_cond_destroy");
    }
    if (pthread_mutex_destroy(&Mutex) < 0)  {
        perror("pthread_mutex_destroy");
    }
}

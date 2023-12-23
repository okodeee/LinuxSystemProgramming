/*
 * dining2.c 
 * Dining philosopher example
 * using pthreads & POSIX semaphore
 * 각 철학자와 critical section에 대하여 semaphore 설정
 * deadlock-free ver. (hold & wait 없음)
 * where starvation can occur
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN)
#define	RIGHT(i)	((i+1)%NUM_MEN)

enum  { THINKING, EATING, HUNGRY };

sem_t	Philosopher[NUM_MEN]; // 전역변수 sem
sem_t	Mutex;
int		State[NUM_MEN];

void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

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
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void
Test(int id)
{
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING 
			&& State[RIGHT(id)] != EATING)  {
		State[id] = EATING;

		// 밥 먹을 수 있으면 signal
		if (sem_post(&Philosopher[id]) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void
PickUp(int id)
{
	if (sem_wait(&Mutex) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	// shared data를 사용하는 critical section
	State[id] = HUNGRY;

	Test(id);

	if (sem_post(&Mutex) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}

	// test에서 밥 먹을 수 있는 상태가 아니면 기다리는 상태
	if (sem_wait(&Philosopher[id]) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

void
PutDown(int id)
{
	if (sem_wait(&Mutex) < 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = THINKING;

	Test(LEFT(id)); // wait하던 철학자는 해당 test에서 signal 받고 eating
	Test(RIGHT(id));

	if (sem_post(&Mutex) < 0)  {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  {

		Thinking(id);

		PickUp(id);

		Eating(id);

		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0 ; i < NUM_MEN ; i++)  {
		// 철학자별로 초기값 0으로 sem 생성, 초기에는 밥 못 먹는 상태
		if (sem_init(&Philosopher[i], 0, 0) < 0)  {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	if (sem_init(&Mutex, 0, 1) < 0)  { //critical section 보호 mutex
		perror("sem_init");
		exit(1);
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&Philosopher[i]) < 0)  {
			perror("sem_destroy");
		}
	}
}

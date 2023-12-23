#include <stdio.h>
#include <pthread.h>


void
PrintMsg(char *msg)
{
	printf("%s", msg);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid1, tid2;
	char		*msg1 = "Hello, ";
	char		*msg2 = "World!\n";
	/* int pthread_create(pthread_t *tid, pthread_attr_t *attr, void *(start_routine)(void *), void *arg), attribute: priority, options */ 
	/* Thread ID: tid1, Thread function: PrintMsg, Thread argument: msg1 argument는 하나만 */
	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	printf("Threads created: tid=%d, %d\n", tid1, tid2);
	
	/* Wait for tid1 to exit */
	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%d, %d\n", tid1, tid2);
}

/*
 * tadd.c
 * 1부터 100까지의 합을 구하는 프로그램
 * 두개의 thread를 생성하여 각각 1~50, 51~100까지의 합을 구하고,
 * main thread가 각 thread가 종료되기를 기다린 후 각각의 합을 더하여 출력 
 */
#include <stdio.h>
#include <pthread.h>


int
Sum50(int start)
{
	int sum = 0;
	for (int i = start; i < start + 50; i++) {
		sum += i;
	}
	pthread_exit(sum);
}


main()
{
	pthread_t	tid1, tid2;
	int		sum1, sum2;
	int		one = 1;
	int		fiftyone = 51;

	if (pthread_create(&tid1, NULL, Sum50, one) < 0) {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&tid2, NULL, Sum50, fiftyone) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	/* Wait for tid1 to exit */
	if (pthread_join(tid1, &sum1) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, &sum2) < 0) {
		perror("pthread_join");
		exit(1);
	}

	printf("1부터 100까지의 합은 %d입니다.\n", sum1 + sum2);
}

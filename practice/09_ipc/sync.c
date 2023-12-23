/*
 * parent와 child 번갈아가면서 수행
 * synclib.c으로 sync 맞춤
 * gcc -o sync sync.c synclib.c
 */
#include <stdio.h>
#include "synclib.h"

#define	NLOOPS	5


main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT(); // 파이프 만들기
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// parent
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i++)  {
			TELL_CHILD();
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();
		}
	}
	// child
	else  {
		for (i = 0 ; i < NLOOPS ; i++)  {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();
			// something
			TELL_PARENT();
			printf("Child: Tell to parent\n");
		}
	}
}

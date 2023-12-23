/*
 * mipc.c
 * memory-mapped file로 구현
 * parent와 child가 번갈아가며 shared memory 값 1 증가
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


int
update(long *ptr)
{
	return ((*ptr)++); //shared memory (count)값 1 증가
}

main()
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	/* 특수 파일: write하면 0, read하면 0 리턴
	“/dev/zero” 파일에 대해 mmap 하는 것은 anonymous mapping 과 같다.
	fork()로 생성된 자식 프로세스들에게 해당 fd를 공유할 수 있다.*/
	if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
	/* area라는 virtual address
	shared memory get, attach로도 만들 수 있어야함 */
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	close(fd);

	TELL_WAIT();
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			if ((counter = update((long *) area)) != i)  {
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
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
	}
}

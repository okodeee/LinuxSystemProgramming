#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"


main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;


	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr;
	pData = ptr + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1; //synchronization
	printf("Sent a request.....");

	while ((*pInt) == 1) // spic1에서 데이터 쓰고 pInt 1로 설정할때까지
		;

	printf("Received reply: %s\n", pData);
}

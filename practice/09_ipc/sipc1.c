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

	// 새로 만들거나 기존의 segment 열기
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	/* 
	ptr의 처음 4byte는 flag로 사용
	처음 초기화 시 모두 0
	ptr + int size(flag) = pData
	*/
	pInt = (int *)ptr; // int size만큼만
	while ((*pInt) == 0)
		;	// sipc2에서 1로 바꾸면 무한 루프 탈출
			// 무한 루프 도는데 어떻게 sipc2 실행? context switch

	pData = ptr + sizeof(int);
	printf("Received request: %s.....", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	*pInt = 0; // synchronization
	printf("Replied.\n");

	sleep(1);	//상대측에서 사용할 수 있도록 텀을 둠

	// physical memory 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
}

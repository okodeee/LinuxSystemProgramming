#include <stdio.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

// process ver.
main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		if ((pid = fork()) < 0)  {
			perror("fork");
			exit(1);
		}
		else if (pid == 0)  {
			// 자식 프로세스에서 실행하고 종료
			DoCmd(cmd);
			exit(0);
		}
// 자식이 DoCmd를 실행할 동안 자신은 새로운 커맨드 입력받음
// if 1이면 기다렸다가 수행
#if 0
		else  {
			wait(NULL);
		}
#endif
	}
}

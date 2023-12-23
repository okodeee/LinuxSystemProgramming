#include <stdio.h>

#define	MAX_CMD		256

void
DoCmd(char *cmd)
{
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

// iteration version using one process
// 다른 사람이 요청을 할 수 없음, 다른 프로세스, 또는 쓰레드가 DoCmd를 실행하도록 하기 위해 cmd_p, cmd_t
main()
{
	char	cmd[MAX_CMD];

	while (1)  {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin);	
		if (cmd[0] == 'q')
			break;

		DoCmd(cmd);
	}
}

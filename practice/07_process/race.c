#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
// 루프를 돌면서 자신의 time quantum을 다 씀 -> 다른 프로세스 실행
		putc(c, stdout);
	}
}

main()
{
	pid_t	pid;

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		CharAtaTime("output from child\n");
	} 
	else  {
		CharAtaTime("output from parent\n");
	}
}
// oouuttppuutt  ffrroomm  pcahrielndt

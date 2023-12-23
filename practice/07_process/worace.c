#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void
CharAtaTime(char *str)
{
	char	*ptr;
	int		c, i;

	setbuf(stdout, NULL);
	for (ptr = str ; c = *ptr++ ; )  {
		for(i = 0 ; i < 999999 ; i++)
			;
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
		// child 프로세스 종료 후 parent 실행
		wait(NULL);
		CharAtaTime("output from parent\n");
	}
}

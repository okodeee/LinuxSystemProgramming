#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main()
{
	pid_t	pid;

	// Create vector
	char	*argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	// execl
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// mycp exec.c exec2.c (실행경로에서 exec.c -> exec2.c)
		/*
		When a process calls an exec function, that process is completely replaced by the new program(text, data, heap, and stack segments) and the new program starts at its and the new program starts at its main function 
		*/
		if (execl("/home/lsp40/practice/05_file-io/mycp", "mycp", "exec.c", "exec2.c", NULL) < 0)  {
			perror("execl");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}

	// execv
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// mycp exec.c exec2.c
		if (execv("/home/lsp40/practice/05_file-io/mycp", argv) < 0)  {
			perror("execv");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}

	// execlp
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		// execute 'env' from PATH
		// env | grep PATH
		// env: 환경 변수를 출력해주는 프로그램
		// env는 usr/bin/env에 존재
		if (execlp("env", "env", NULL) < 0)  {
			perror("execlp");
			exit(1);
		}
	}
	if (waitpid(pid, NULL, 0) < 0)  {
		perror("waitpid");
		exit(1);
	}

}


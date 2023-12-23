/*
 * mysystem.c
 * int mysystem(char *cmd) 함수 구현
 * 동작은 "system" system call과 동일
 * fork, execv, waitpid system call을 이용
 */
#include <stdio.h>
#include <sys/types.h> //waitpid
#include <unistd.h> // exec

int
mysystem(char *cmd) {
	pid_t	pid;
	char	*path = "/bin/sh";
	char	*argv[] = {"sh", "-c", cmd, NULL};
	int 	status;

	if ((pid = fork()) < 0) {
    	return -1;
		// fork failed
	}
    else if (pid == 0)  { // child process
        if (execv(path, argv) < 0) {
		// execl("/bin/sh", "sh", "-c", command, (char *) 0);
        	exit(127); //execv failed
		}
    }
    if (waitpid(pid, &status, 0) < 0)  {
    	return -1;
    }
	return status;
}


main()
{
	int	status;

	if ((status = mysystem("ls -al")) < 0) {
		perror("mysystem");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("date")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("who")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}

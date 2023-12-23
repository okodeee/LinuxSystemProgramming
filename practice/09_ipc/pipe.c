#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	if (pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	// child
	else if (pid == 0)  {
		// write 사용 안 함
		close(fd[1]);
		printf("Child : Wait for parent to send data\n");
		/* child가 먼저 실행될 때 read하면 waiting 상태로
			운영체제가 알아서 sync 맞춰줌
			fd[0](read)	MAX_BUF만큼 buf에 저장
		*/
		if ((n = read(fd[0], buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout); // \n이 없기 때문에 fflush 이후로 출력
		write(STDOUT_FILENO, buf, n); // printf랑 동일하게 화면에 출력
	}
	// parent
	else  {
		// read 사용 안 함
		close(fd[0]);
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		write(fd[1], buf, strlen(buf) + 1);
									// null char 포함
	}

	exit(0);
}

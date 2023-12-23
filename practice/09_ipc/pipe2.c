// child도 parent에게 메시지를 전달하도록 pipe.c를 변경
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define	MAX_BUF	128

main()
{
	int		i, n, fd1[2], fd2[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	if (pipe(fd1) < 0 || pipe(fd2) < 0)  {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  { //child
		close(fd1[1]);
		close(fd2[0]);
		
		for (i = 0; i < 5; i++) {
			//WAIT_PARENT
			printf("Child: Wait for parent to send data\n");
			if ((n = read(fd1[0], buf, MAX_BUF)) < 0)  {
				perror("read");
				exit(1);
			}
			printf("Child: Received data from parent: ");
			fflush(stdout);
			write(STDOUT_FILENO, buf, n);

			//TELL PARENT
			strcpy(buf, "Bye, World!\n");
			printf("Child: Send data to parent\n");
			write(fd2[1], buf, strlen(buf)+1);
		}
	}
	else  { //parent
		close(fd1[0]);
		close(fd2[1]);
		
		for (i = 0; i < 5; i++) {
			//TELL_CHILD
			strcpy(buf, "Hello, World!\n");
			printf("Parent: Send data to child\n");
			write(fd1[1], buf, strlen(buf)+1);

			//WAIT_CHILD
			printf("Parent: Wait for child to send data\n");
			if ((n = read(fd2[0], buf, MAX_BUF)) < 0)  {
				perror("read");
				exit(1);
			}
			printf("Parent: Received data from child: ");
			fflush(stdout);
			write(STDOUT_FILENO, buf, n);
		}
	}

	exit(0);
}

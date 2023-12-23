#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int 
main() 
{
	int fd[2];
	pid_t child;

	if(pipe(fd) == -1)  {
		perror("pipe");
		exit(1);
	}
 
	child = fork();
	if(child == (pid_t)(-1))  {
		perror("fork");
		exit(1);
	}
	// child
	else if(child == (pid_t)0) {
        close(1); // stdout
		close(fd[0]); // child는 read 안 함

		if(dup(fd[1]) == -1)  { /* fd[1] = 1(stdout) 
					사용하지 않는 file descriptor와 dup */
			perror("dup");
			exit(1);
		}
		// 환경변수로 ls -l 실행하여 출력 -> fd[1]
        if((execlp("ls", "ls", "-l", NULL)) == -1)  {
        	perror("execlp");
			exit(1);
		}
	}
	// parent 
	else {
        close(0);
		close(fd[1]); // parent는 write 안 함
        	
        if(dup2(fd[0], 0) == -1)  { // dup(fd[0]) 와 동일
        	perror("dup");
			exit(1);
		}

        if((execlp("more", "more", NULL)) == -1)  {
        	perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}

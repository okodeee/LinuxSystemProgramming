#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"


void
SigIntHandler(int signo) //프로그램 종료 시 서버파이프 삭제
{
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}

	exit(0);
}


main()
{
	int			fd, cfd, n;
	MsgType		msg;

	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	/* 서버파이프 만들기
		SERV_FIFO ./.fifo로 define
		int mkfifo(char *pathname, mode_t mode); */
	if (mkfifo(SERV_FIFO, 0600) < 0)  {
		if (errno != EEXIST)  { //이미 존재하는 경우는 에러 아님
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	while (1)  {
		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
		//메세지 기다림
			if (errno == EINTR)  {
				continue;
			}
			else  {
				perror("read");
				exit(1);
			}
		}
		//메세지 받음
		printf("Received request: %s.....", msg.data);

		//client로 메세지 보내기
		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid());
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd);
		printf("Replied.\n");
	}
}

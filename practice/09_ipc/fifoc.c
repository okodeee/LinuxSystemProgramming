#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"


main()
{
	char		fname[MAX_FIFO_NAME];
	int			fd, sfd, n;
	MsgType		msg;

	//client-specific fifo 만들기
	sprintf(fname, ".fifo%d", getpid());
	if (mkfifo(fname, 0600) < 0)  {
		perror("mkfifo");
		exit(1);
	}

	//receive from server
	if ((fd = open(fname, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}
	//send to server
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// 서버로 메세지 전달
	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid());
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");

	// 서버가 보낸 메세지 기다리기
	if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);

	//내가 만든 fifo 삭제
	if (remove(fname) < 0)  {
		perror("remove");
		exit(1);
	}
}

// tcps.c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


int	Sockfd; // signal handler에서 사용하도록 하기 위해 전역변수 설정

void
CloseServer() // signal handler
{
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; // IPv4 주소
	MsgType				msg;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");		// create a socket, TCP: SOCK_STREAM
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // 0으로 초기화
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP address
	// server IP address가 여러 개일 수 있는데 어떤 것이든 처리 (any)
	servAddr.sin_port = htons(SERV_TCP_PORT); // port number

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {	// bind a name to an unnamed socket, my socket address
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // listen connections on a socket

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // TCP connection이 맺어지도록 기다림, cliAddr(상대 측 주소 받기)
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		// read
		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received request: %s.....", msg.data); 
		//write
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		usleep(10000);
		close(newSockfd);
	}
}

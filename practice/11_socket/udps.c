/* 
 * udps.c
 *========================
 * TCP와 차이점
 * socket system call type: SOCK_DGRAM
 * connectionless
 * recvfrom, sendto
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"


int	Sockfd;

void
CloseServer()
{
	close(Sockfd);
	printf("\nUDP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket"); // UDP: SOCK_DGRAM
		exit(1);
	}

	// socket address
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // bind a name to an unnamed socket
		perror("bind");
		exit(1);
	}

	// UDP는 connectionless

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  { // connection 없이 바로 recvfrom
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
	// cliAddr 주소 받음
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(Sockfd, (char *)&msg, sizeof(msg),
					0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}

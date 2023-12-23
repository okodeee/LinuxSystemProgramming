/*
 * chatc_t.c
 * select system call로 multiplexing 하지 않고, multi-threads로 처리
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "chat.h"

#define	MAX_BUF		256

int		Sockfd;


void
SendChat(void) 
{
	char	buf[MAX_BUF];
	int		n;
	while (1) {
		fgets(buf, MAX_BUF, stdin);
		if ((n = send(Sockfd, buf, strlen(buf)+1, 0)) < 0) {
			perror("send");
			pthread_exit(NULL);
		}
	}
}


void
RecvChat(void)
{
	char	buf[MAX_BUF];
	int		n;

	while (1) {
		if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0) {
			perror("recv");
			pthread_exit(NULL);
		}
		if (n == 0) {
			fprintf(stderr, "Server terminated.....\n");
			close(Sockfd);
			pthread_exit(NULL);
		}
		printf("%s", buf);
	}
}


void
ChatClient(void)
{
	char		buf[MAX_BUF];
	pthread_t	tid1, tid2;

	printf("Enter ID: ");
	fflush(stdout);
	fgets(buf, MAX_BUF, stdin);
	*strchr(buf, '\n') = '\0';
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {
		perror("send");
		exit(1);
	}
	printf("Press ^C to exit\n");

	if (pthread_create(&tid1, NULL, (void*)SendChat, NULL) < 0) {
		perror("pthread_create");
        exit(1);
	}
	if (pthread_create(&tid2, NULL, (void*)RecvChat, NULL) < 0) {
		perror("pthread_create");
        exit(1);
	}

	if (pthread_join(tid1, NULL) != 0) {
        perror("pthread_join");
        exit(1);
	}
	if (pthread_join(tid2, NULL) != 0) {
        perror("pthread_join");
        exit(1);
	}
}
	

void
CloseClient(int signo)
{
	close(Sockfd);
	printf("\nChat client terminated.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	struct sockaddr_in	servAddr;
	struct hostent		*hp;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  {
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	signal(SIGINT, CloseClient);

	ChatClient();
}

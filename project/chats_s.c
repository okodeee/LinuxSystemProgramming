/*
 * chats_s.c
 * thread를 생성하지 않고, select system call로 multiplexing 처리
 */
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256

typedef	struct  {
	int			sockfd;
	int			inUse;
	pthread_t	tid;
	char		uid[MAX_ID];
}
	ClientType;

int				Sockfd;

ClientType		Client[MAX_CLIENT];


int
GetID()
{
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (! Client[i].inUse)  {
			Client[i].inUse = 1;
			return i;
		}
	}
}


void
SendToOtherClients(int id, char *buf)
{
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse && (i != id))  {
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
				perror("send");
				exit(1);
			}
		}
	}
}


void
CloseServer(int signo)
{
	int		i;

	close(Sockfd);

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			close(Client[i].sockfd);
		}
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	char				buf[MAX_BUF];
	int					newSockfd, cliAddrLen, id, count, n, one = 1;
	struct sockaddr_in	cliAddr, servAddr;
	fd_set				fdset;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("Chat server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
        FD_ZERO(&fdset);            // clear all bits in fdset
        FD_SET(Sockfd, &fdset);  // turn the bit for fd on
		for (id = 0; id < MAX_CLIENT; id++) {
			if (Client[id].inUse) {
				FD_SET(Client[id].sockfd, &fdset);
			}
		}
        if ((count = select(10, &fdset, (fd_set *)NULL, (fd_set *)NULL,
            (struct timeval *)NULL)) < 0)  { // read용
            perror("select");
            exit(1);
        } // 도착 개수 만큼

        while (count--)  {
            if (FD_ISSET(Sockfd, &fdset))  { // 어떤 소켓에서 왔는지 확인
				newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
				if (newSockfd < 0)  {
					perror("accept");
					exit(1);
				}

				id = GetID();
				Client[id].sockfd = newSockfd;


    			if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  {
        			perror("recv");
        			exit(1);
    			}
    			printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);
			}
			else {
				for (id = 0; id < MAX_CLIENT; id++) {
					if (Client[id].inUse == 0) {
						continue;
					}

					if (FD_ISSET(Client[id].sockfd, &fdset)) {
				
        				if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  {
            				perror("recv");
            				exit(1);
        				}
        				if (n == 0)  {
            				printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

							FD_CLR(Client[id].sockfd, &fdset);
            				close(Client[id].sockfd);
            				Client[id].inUse = 0;

            				strcpy(buf, "log-out.....\n");
            				SendToOtherClients(id, buf);
        				}
						else {
        					SendToOtherClients(id, buf);
    					}
					}
				}
			}
		}
	}
}

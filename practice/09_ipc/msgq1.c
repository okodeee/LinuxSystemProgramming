#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"

#define	MSG_MODE	(0600 | IPC_CREAT)


main()
{
	int		msqid, n;
	MsgType	msg;


	if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
		perror("msgget");
		exit(1);
	}

	if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), 0, 0)) < 0)  {
		perror("msgrcv");
		exit(1);
	}
	printf("Received request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
		perror("msgsnd");
		exit(1);
	}
	printf("Replied.\n");

	sleep(1);

	if (msgctl(msqid, IPC_RMID, 0) < 0)  {
		perror("msgctl");
		exit(1);
	}
}

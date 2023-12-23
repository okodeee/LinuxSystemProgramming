#include <stdio.h>
#include <signal.h>

// 해당 파일 외에서는 참조 불가능하도록 static 설정
static unsigned int	AlarmSecs;


void
SigAlarmHandler(int signo)
{
	/* LINUX에서는 불필요
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}
	*/

	alarm(AlarmSecs);

	/* Do something */
	printf(".");
	fflush(stdout); // '\n'이 안 나와도 출력, buffer cache 비움

	return;
}

int
SetPeriodicAlarm(unsigned int nsecs)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return -1;
	}

	AlarmSecs = nsecs;

	alarm(nsecs);

	return 0;
}

main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1);

	for ( ; ; )
		pause();
}

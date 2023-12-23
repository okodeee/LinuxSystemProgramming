#include <stdio.h>
#include <signal.h>

void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int
mysleep(unsigned int nsecs)
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		return nsecs;
	}

	/* alarm: set an alarm clock for delivery of a signal */
	alarm(nsecs);

	pause(); // wait for signal

	return alarm(0);
	// 알람이 아닌 다른 시그널에 의해 pause를 빠져나올 때
	/* alarm() returns the number of seconds  remaining until any previously scheduled alarm was due to be delivered, or zero if there was no previously scheduled alarm. */
}

main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5);
}

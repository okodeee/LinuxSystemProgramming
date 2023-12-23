#include <stdio.h>
#include <signal.h>

void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

main()
{
	/* SIGINT signal handler: SigIntHandler */
	/* signal: register a signal handler */
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for ( ; ; )
		pause(); // wait for signal
}

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void
myusleep(int secs, int usecs)
{
	struct timeval	tm;

	tm.tv_sec = secs;
	tm.tv_usec = usecs;
	// select system call을 특정 시간(상대 시간) 동안 sleep
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0)  {
		perror("select");
		return;
	}
}

main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}

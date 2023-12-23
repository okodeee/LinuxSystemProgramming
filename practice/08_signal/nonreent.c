#include <stdio.h>
#include <signal.h>
#include <pwd.h>


void
MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	ualarm(100000);

	printf("in signal handler\n");

	if ((rootptr = getpwnam("root")) == NULL)  {
					/* static struct passwd pw;
					 return &pw; */
		perror("getpwnam");
		exit(1);
	}

	return;
}

main()
{
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	for ( ; ; )  {
		// cjs 유저에 대한 정보 가져오기
		if ((ptr = getpwnam("cjs")) == NULL)  {
			perror("getpwnam");
			exit(1);
		}

		if (strcmp(ptr->pw_name, "cjs") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}

/* 
gdb a.out
break main
run
n
*/

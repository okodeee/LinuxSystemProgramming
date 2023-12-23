#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	char	buf[MAX_BUF];

	// get current working directory
	if (getcwd(buf, MAX_BUF) < 0)  {
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
}

#include <stdio.h>
#include <stdlib.h>

void
myexit1()
{
	printf("first exit handler\n");
}

void
myexit2()
{
	printf("second exit handler\n");
}

main()
{
	/* Add myexit2 to exit functions */
	if (atexit(myexit2) != 0)  {
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0)  {
		perror("atexit");
		exit(1);
	}

	printf("main is done\n");
}

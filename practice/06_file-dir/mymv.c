#include <stdio.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	
	// rename a file or directory
	if (rename(argv[1], argv[2]) < 0)  {
		perror("rename");
		exit(1);
	}
}

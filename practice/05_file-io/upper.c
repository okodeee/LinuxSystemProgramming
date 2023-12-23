#include <stdio.h>


main(int argc, char *argv[])
{	
	// Declaration of a pointer fo FILE type
	FILE	*fp;
	int		ch;	

	if (argc != 2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}
	
	// Open a stream
	if ((fp = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}
	
	// Read the next character from stream and print uppercase
	while ((ch = fgetc(fp)) != EOF) {
		printf("%c", toupper(ch));
	}

	// Close a stream
	fclose(fp);
}

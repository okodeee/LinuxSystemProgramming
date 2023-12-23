#include <stdio.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	// file pointer 선언
	FILE	*fp;
	char	buf[MAX_BUF];
	int		line;

	if (argc != 2)  {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}
	// open a text file for reading(rt)
	if ((fp = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	line = 1;
	// fgets: file의 끝을 만나면 NULL return
	while (fgets(buf, MAX_BUF, fp))  {
		printf("%4d: %s", line++, buf);
	}
	// close a file
	fclose(fp);
}

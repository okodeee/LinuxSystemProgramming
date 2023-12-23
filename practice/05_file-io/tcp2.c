#include <stdio.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// open a text file for reading (The file must exist)
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// open a text file for writing If the file already exists, its contents are destroyed
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// EOF가 아닐 때까지 line 읽고 쓰기
	while (fgets(buf, MAX_BUF, src) != NULL)  {
		fputs(buf, dst);
	}

	fclose(src);
	fclose(dst);
}

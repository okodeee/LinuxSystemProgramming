#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	int		ch;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// open a text file for reading
	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// open a text file for writing. If the file already exists, its contents are destroyed
	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// EOF가 아닐 때까지 character 읽고 쓰기
	while ((ch = fgetc(src)) != EOF)  {
		fputc(ch, dst);
	}

	fclose(src);
	fclose(dst);
}

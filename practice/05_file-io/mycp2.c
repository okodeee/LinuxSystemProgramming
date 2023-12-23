#include <stdio.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// open a binary file for reading (The file must exist)
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// open an empty binary file for writing If the file already exists, its contents are destroyed
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// 1 byte 단위로 read write
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		fwrite(buf, 1, count, dst);
	}

	fclose(src);
	fclose(dst);
}

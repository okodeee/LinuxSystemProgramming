#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024

long
filesize(FILE *fp)
{
	long	cur, size;
	// ftell: return current file position
	// 처음 위치와 끝 위치를 이용해서 filesize 구하기
	cur = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, cur, SEEK_SET);
	return(size);
}

main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}
	// Open a binary file for reading. (The file must exist.)
	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// Open an empty binary file for writing. If the file already exists, its contents are destroyed.
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	size = filesize(src) / 2;
	// filesize의 반만큼 dst1에 write
	while (size > 0)  {
		count = (size > MAX_BUF) ?  MAX_BUF : size;
		// halfsize를 넘기면 안되기 때문에 위 과정을 수행
		fread(buf, 1, count, src); 
		fwrite(buf, 1, count, dst1);
		size -= count;
	}
	// 나머지 반 dst2에 write
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}

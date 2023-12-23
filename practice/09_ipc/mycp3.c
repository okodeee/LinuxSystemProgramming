/* mycp3.c
 * memory-mapped file로 cp 구현
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

main(int argc, char *argv[])
{
	int			fdin, fdout;
	char		*src, *dst;
	struct stat	statbuf;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	/* dst file open with read-write mode,
	create a new one if it does not exist and overwrite it if it exists
	0644 -> 0 means an octal number */
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	// file size 가져오기
	if (fstat(fdin, &statbuf) < 0)  {
		perror("fstat");
		exit(1);
	}
	// dst file에 매핑할 게 없어서 임의로 길이 늘리기
	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	write(fdout, "", 1); // 	|---lseek---|'\0'|

		// src라는 virtual address
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) 
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}

	// memory 상에서 복사하면 file copy 일어남
	memcpy(dst, src, statbuf.st_size);

	close(fdin);
	close(fdout);
}

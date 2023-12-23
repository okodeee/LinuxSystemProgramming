#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	// Exit the program if there are no 3 input arguments
	if (argc != 3)  {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	// src file open with readonly mode, exit if error occurs
	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}
	/*
	dst file open with writeonly mode,
	create a new one if it does not exist and overwrite it if it exists
	0644 -> 0 means an octal number
	*/
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}
	// Read and write over and over again by MAX_BUF bytes until it is not EOF
	while (count = read(fd1, buf, MAX_BUF) > 0)  {
		write(fd2, buf, count);
	}
	// close a file descriptor
	close(fd1);
	close(fd2);
}

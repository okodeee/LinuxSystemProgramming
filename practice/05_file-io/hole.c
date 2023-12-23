#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int 	fd;
	char	ch = '\0';
	// equivalent to char buf[1] = {'\0'};

	// create a new file, read mode
	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");
		exit(1);
	}
	// offset 1023 from the beginning
	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek");
		exit(1);
	}
	
	write(fd, &ch, 1);
	// equivalent to write(fd, buf, 1);

	close(fd);
}

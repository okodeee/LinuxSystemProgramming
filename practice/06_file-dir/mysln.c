#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
/* Editing */
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	// make a new name for a file (symbolic link) => ln -s와 동일
	/*
	-rw-r--r-- 1 lsp40 lsp 239 Oct 28 15:33 mysln.c
	lrwxrwxrwx 1 lsp40 lsp   7 Oct 28 15:34 mysln.c.ln -> mysln.c
	*/
	if (symlink(argv[1], argv[2]) < 0)  {
		perror("link");
		exit(1);
	}
}

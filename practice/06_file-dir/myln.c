#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
/* Editing */
	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	// make a new name for a file (hard link) => ln과 동일
	/*
	-rw-r--r-- 2 lsp40 lsp 246 Oct 28 15:26 myln.c
	-rw-r--r-- 2 lsp40 lsp 246 Oct 28 15:26 myln.c.ln
	*/
	if (link(argv[1], argv[2]) < 0)  {
		perror("link");
		exit(1);
	}
}

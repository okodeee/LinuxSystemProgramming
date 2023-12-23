#include <stdio.h>

/*
	argc: 파일 개수
	argv: character pointer array
	ex) ./mycp mycp.c yourcp.c
	argv[0] = "./mycp"
	argv[1] = "mycp.c"
	argv[2] = "yourcp.c"
	argv[3] = ""
*/
main(int argc, char *argv[])
{
	int		i;

	for (i = 0 ; i < argc ; i++)
		printf("argv[%d]=%s\n", i, argv[i]);
}

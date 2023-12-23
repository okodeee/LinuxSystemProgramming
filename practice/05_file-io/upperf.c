#include <stdio.h>

void
upper(char *fname)
{
    FILE    *src, *dst;
	char 	fdest[40];
    int     ch;

    if ((src = fopen(fname, "rt")) == NULL)  {
        perror("fopen");
        exit(1);
    }
	
	strcpy(fdest, fname);
	strcat(fdest, ".u");

    if ((dst = fopen(fdest, "wt")) == NULL)  {
        perror("fopen");
        exit(1);
    }

    while ((ch = fgetc(src)) != EOF)  {
        fputc(toupper(ch), dst);
    }

    fclose(src);
    fclose(dst);
}

main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s filename\n", argv[0]);
	}	

	upper(argv[1]);
}

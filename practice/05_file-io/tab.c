#include <stdio.h>

enum { FALSE, TRUE };

void
conv(char *fname)
{
	FILE	*src, *dst;
	char	fdest[40];
	int		ch, first;
	// Open a text file for reading. (The file must exist.)
	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}

	// fname을 fdest에 복사하고 .t를 붙인다. 작성할 파일명.
	strcpy(fdest, fname);
	strcat(fdest, ".t");
	// Open a text file for writing.
	// If the file already exists, its contents are destroyed.
	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	}

	first = TRUE;
	// EOF가 아닐 때까지 한 문자씩 읽기
	while ((ch = fgetc(src)) != EOF)  {
		// '\t'을 space * 4로 변경 
		if (first && ch == '\t')  {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		// 다른 문자는 그대로 쓰기
		else  {
			fputc(ch, dst);
			// 문장 중간의 tap을 구별하기 위해
			if (first)
				first = FALSE;
			// 줄 바뀔 때마다
			if (ch == '\n')
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);
}

main(int argc, char *argv[])
{
	while (--argc)  {
		conv(argv[argc]);
	}
}

#include <stdio.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	Record	rec;

	if (argc != 3)  {
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	// record 단위
	while (fgets(rec.stud, SMAX, src))  {
		// The strchr() functions  return  a  pointer  to  the first occurrence of the character or NULL if the character is not found.
		// 문자열을 읽어오고 줄바꿈을 널로 변경해서 레코드 속성에 저장
		*strchr(rec.stud, '\n') = '\0';
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0';
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';
		
		// 하나의 record 단위로 dst에 쓰기
		fwrite(&rec, sizeof(rec), 1, dst);
	}

	fclose(src);
	fclose(dst);
}

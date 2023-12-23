#include <stdio.h>
#include <unistd.h>
#include "record.h"

void
print_record(Record *rp)
{
	// print string of record.stud, record.num, record.dept
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

main(int argc, char *argv[])
{
	FILE	*fp;
	Record	rec;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}
	// Open a binary file for both reading and writing. (The file must exist.)
	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");
	// fread(1st argument: pointer)
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}
	// set file position to the beginning of the file
	rewind(fp);
	// getchar() is equivalent to getc(stdin).
	getchar();

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");
	// 3번째 record로 이동하고 record 단위로 read
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar();

	printf("-----Updated Record List-----\n");

	// record 단위로 읽어오고 값을 변경해서 쓰기
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");
	// rec.dept = "Movie"
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	rewind(fp);
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec);
	}

	fclose(fp);
}

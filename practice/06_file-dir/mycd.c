#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 2)  {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	// change working directory
	if (chdir(argv[1]) < 0)  {
		perror("chdir");
		exit(1);
	}
}

/* mycd(예제 코드 컴파일한 실행파일)은 셸의 자식 프로세스가 된다. 실행되어 있는 상태에서 mycd 프로세스의 작업경로가 바뀌는 것이지 셸을 영향을 받지 않는다. */

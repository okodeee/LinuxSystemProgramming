#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	// umask() sets the calling process's file mode creation mask (umask) to mask
	// 아무것도 mask 하지 않음
	umask(0);
					// on modes bit OR 연산
					// 실행결과 rw-rw-rw-
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}

	/* Set file mode creation mask: S_IRGRP, S_IWGRP, S_IROTH, S_IWOTH */
	// 해당 모드에 대해서 mask
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
					// 실행결과 rw-------
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}
}

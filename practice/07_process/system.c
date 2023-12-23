#include <stdio.h>
#include <stdlib.h>

main()
{
	int		status;

	// date: 날짜 출력
	if ((status = system("date")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	if ((status = system("nosuchcommand")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	// who command: 현재 로그인한 사용자
	/* $ who; exit 44 */
	if ((status = system("who; exit 44")) < 0)  {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}

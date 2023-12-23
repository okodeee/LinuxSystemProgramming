#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int	Pfd1[2], Pfd2[2];
// Pfd1: parent -> child
// Pfd2: child -> parent
// 내용은 중요하지 않고 누가 보냈는지 표시

void
TELL_WAIT(void)
{
	// 쌍방향을 위해 pipe 2개
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
		perror("pipe");
		exit(1);
	}
}

void
TELL_PARENT(void)
{
	// child -> parent write
	if (write(Pfd2[1], "c", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void
WAIT_PARENT(void)
{
	char	c;

	// child가 read
	if (read(Pfd1[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'p')  {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void
TELL_CHILD(void)
{
	if (write(Pfd1[1], "p", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void
WAIT_CHILD(void)
{
	char	c;

	if (read(Pfd2[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'c')  {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}

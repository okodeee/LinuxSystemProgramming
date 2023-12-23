#include <stdio.h>

main()
{
	char c = 'a', s[] = "hello";
	int i = 100; long l = 99999;
	float f = 3.14; double d = 99.999;
	int *p = &i;
// format string	
	printf("Output: %c %s %d %#X %ld %.4f %.2lf %p\n", c, s, i, i, l, f, d, p); 
// c: character, s: string, d: decimal(int), X: hexa(16) & uppercase | #: 0X__ 표기, .4f: 소수점 아래 4자리까지 표현, p: 포인터변수
	putchar(c);
	puts(s);
}

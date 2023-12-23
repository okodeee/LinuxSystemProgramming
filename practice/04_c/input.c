#include <stdio.h>

main()
{
	char c, s[80];
	int i; long l;
	float f; double d;

// c에서는 레퍼런스 타입x -> 포인터 변수
// s: character array라서 그 자체로 포인터	
	scanf("%c %s %d %ld %f %lf", &c, s, &i, &l, &f, &d); 
	printf("Output: %c %s %d %ld %.4f %.2lf\n", c, s, i, l, f, d); 

// character 입력받기
	c = getchar();
	putchar(c);

// string(characters) 입력받기
	gets(s);
	puts(s);
}

#include <stdio.h>

int
strlen_p(char *str)
{
	int	len = 0;

	while (*str++)  {
	// NULL이 아닐 때까지만
		len++;
	// 길이 +1
	}
	return len;
}

int
strlen_a(char str[])
{
	int i;

	for (i = 0 ; str[i] != '\0' ; i++)
	// array 로 접근해서 NULL이 아닐 때까지만
		;
	return i;
}

void
strcpy_p(char *dst, char *src)
{
	while (*src)  {
		*dst++ = *src++;
	}
	// src가 NULL 이전일때까지 dst에 copy
	*dst = *src;
	// NULL copy
}

void
strcpy_a(char dst[], char src[])
{
	int i;

	for (i = 0 ; src[i] != '\0' ; i++)
		dst[i] = src[i];
	// src가 NULL 이전일때까지 dst에 copy
	dst[i] = src[i];
	// NULL copy
}

void
strcat_p(char *dst, char *src)
{
	while (*dst++)
		;
	// dst는 동일하게 존재
	dst--;
	// while문을 통해 NULL 다음 주소를 가리키고 있으므로 dst--
	while (*src)  {
		*dst++ = *src++;
	}
	// src copy
	*dst = *src;
	// NULL copy
}

void
strcat_a(char dst[], char src[])
{
	int i, j;

	for (i = 0 ; dst[i] != '\0' ; i++)
		;
	for (j = 0 ; src[j] != '\0' ; j++)
		dst[i+j] = src[j];
	// dst의 NULL 주소부터 src내용 copy
	dst[i+j] = src[j];
	// NULL copy
}

int
strcmp_p(char *dst, char *src)
{
	while (*dst || *src) {
	// *dst, *src 둘 다 NULL이 아닐 때까지 반복
	// *dst++, *src++를 사용하면 첫 번째 문자 비교 불가
		if (*dst != *src) {
			return *dst - *src;
			// 다를 때 dst > src 이면 positive, 반대면 negative 리턴
		}
		dst++; src++;
	}
	return 0;
	// 같은 문자열일 때 0 리턴
}

int
strcmp_a(char dst[], char src[])
{
	int i;

	for (i = 0; dst[i] || src[i]; i++) {
	// dst[i], src[i] 둘 다 NULL이 아닐 때까지 반복
		if (dst[i] != src[i]) {
			return dst[i] - src[i];
			// 다를 때 dst > src 이면 positive, 반대면 negative 리턴
		}	
	}
	return 0;
	// 같은 문자열일 때 0 리턴
}

main()
{
	int		len1, len2;
	char	str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);

	int		cmp1, cmp2;
	cmp1 = strcmp_p(str1, str2);
	cmp2 = strcmp_a(str1, str2);
	printf("strcmp: p=%d, a=%d\n", cmp1, cmp2);
}

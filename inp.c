#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int input(int *n)
{
	char c;
	int num;
	int scr = scanf("%d%c", &num, &c);

	if (scr == EOF)
	{
		return EOF;
	}

	if ((scr != 2) || (c != '\n'))
	{
		scanf("%*[^\n]%*c");
		printf("некорректный ввод\n");
		return 2;
	}

	if ((scr = 2) && (c == '\n'))
	{
		if (num > 0)
		{
			*n = num;
			return 0;	
		}
		else
		{
			printf("неверный диапазон\n");
			return 2;
		}
	}

	return -1;
}

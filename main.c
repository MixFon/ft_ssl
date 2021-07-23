
#include "stdio.h"
#include "stdlib.h"

int main(void)
{
	char *temp;
	int i;

	i = 0;
	while (++i < 10)
	{
		temp = (char *)malloc(sizeof(char) * 10);
		printf("Hwllo\n");
	}
	return (0);
}

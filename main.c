
#include "stdio.h"
#include "stdlib.h"

int main(void)
{
	int i;
	char *temp;

	i = 0;
	while (++i < 10)
	{
		temp = (char *)malloc(10);
		free(temp);
	}
	return (0);
}

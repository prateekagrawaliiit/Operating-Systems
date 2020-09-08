	#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int asc(int a, int b)
{
	if(a>b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int desc(int a, int b)
{
	if(a<b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


int main(int argc, char* argv[])
{
	int array[100], n, c, d, swap;
	int inc = 0;
	n = atoi(argv[1]);
	inc = atoi(argv[2]);
	int (*order[])(int, int) = {desc, asc};


	for (c = 0; c < n; c++)
		array[c] = atoi(argv[c + 3]);

	for (c = 0 ; c < n - 1; c++)
	{
		for (d = 0 ; d < n - c - 1; d++)
		{
        if ((*order[inc])(array[d],array[d + 1])) /* For decreasing order use < */
			{
				swap       = array[d];
				array[d]   = array[d + 1];
				array[d + 1] = swap;
			}
		}
	}    printf("Output:\n");    for (c = 0; c < n; c++)
	printf("%d\n", array[c]);


}

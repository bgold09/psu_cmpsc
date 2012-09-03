#define SVID_SOURCE
#include <stdio.h>

int main() 
{
	int i, j; 
	size_t size;
	int A[] = {0, 1, 2, 4, 17, 12, 6, 9, 10, 11, 22, 
		   23, 24, 8, 5, 13, 14, 15, 16, 18, 19};
	
	size = sizeof(A) / sizeof(int);

	printf("size = %zu\n", size);

	i = 1;
	j = 2;

	printf("(A + i)[j] = %d\n", (A + i)[j]);
	printf("A[i + j] = %d\n", A[i + j]);

	printf("the value of expression `i = 17' = %d\n", (i = 17));
	printf("the value of expression `i = 17, j = 8' = %d\n", (i = 17, j = 8));
	printf("the value of expression `i = 7, j = 15, i < j "
			"? \"Yes\" : \"No\"' = %s\n", 
			(i = 7, j = 15, i < j ? "Yes" : "No"));

	return 0;
}

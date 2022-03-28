#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int j;
	int i, k;
	i = atoi(argv[1]);
	k = atoi(argv[2]);
	
	printf("%d + %d = %d\n",i, k, i+k);
	printf("%d - %d = %d\n",i, k, i-k); 
	printf("%d * %d = %d\n",i, k, i*k); 
	printf("%d / %d = %.2f\n",i, k, (float)i/k);

	/*printf("argc = %d\n",argc);
	
	for(j =0; j<argc; j++)
	{
		printf("argv[%d] =  %s \n",j,argv[j]);
	}*/
 
	return 1;
 
}

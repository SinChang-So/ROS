#include <stdio.h>

int main(int argc, char* argv[])
{
	int i, k;
	
	printf("enter two num : ");
	scanf("%d %d", &i, &k);
	
	printf("%d + %d = %d\n",i, k, i+k);
	printf("%d - %d = %d\n",i, k, i-k); 
	printf("%d * %d = %d\n",i, k, i*k); 
	printf("%d / %d = %.2f\n",i, k, (float)i/k);  

	/*printf("argc = %d\n",argc);
	
	for(i =0; i<argc; i++)
	{
		printf("argv[%d] =  %s \n",i,argv[i]);
	}*/
 
	return 1;
 
}

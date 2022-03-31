#include <stdio.h>
#include <math.h>

float dividend(int x[], float sum_x, int y[], float sum_y);
float divisor(int x[], float avg_x);
int main(void)
{
	int x[] = {2,4,6,8};
	int y[] = {81,93,91,97};
	float sum_x=0.0, sum_y=0.0;
	float avg_x =0.0, avg_y=0.0;
	float k = 0.0, j=0.0;
	int index_x = sizeof(x)/sizeof(int);
	int index_y = sizeof(y)/sizeof(int);

	for(int i=0; i<4; i++)
	{
		sum_x +=x[i];
		sum_y +=y[i];
	}

	avg_x = sum_x/index_x;
	avg_y = sum_y/index_y;

	//printf("%.2f   %.2f", avg_x, avg_y);
	float a = 0.0;
	float b = 0.0;

	k = dividend(x,avg_x,y,avg_y);
	j = divisor(x,avg_x); 

	a = k/j;
	b = avg_y - (avg_x*a);
	printf("기울기 : %.2lf\n",a );
	printf("Y 절편 : %.2lf\n",b );
	return 0;
}

float dividend(int x[], float avg_x, int y[], float avg_y)
{
	float d = 0.0;
	for(int i=0; i<4; i++)
	{
		d += (x[i]-avg_x)*(y[i]-avg_y);
	}
	
	return d;
}
float divisor(int x[], float avg_x)
{
	float d = 0.0;
	for(int i=0; i<4; i++)
	{
		d += pow((x[i]-avg_x),2);
	}
	
	return d;
}
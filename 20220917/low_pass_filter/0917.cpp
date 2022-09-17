#include <stdio.h>

#define MASK 5

double data[] = { 0, };
double recent_data = 0;
double prev_averge = 0.0;
//int index = 0;

void input_data(int mask, int INDEX);
double low_pass_filter(double RECENT_DATA, double alpha);

int main(void)
{
	int index = 0;
	double low_pass_filter_data=0.0;
	while (1)
	{
		input_data(MASK,index);
		recent_data = data[index];
		index++;

		if (index >= MASK)
		{
			index = 0;
		}
		printf("\n");
		printf("recent_data : %.2lf\n", recent_data);
		low_pass_filter_data = low_pass_filter(recent_data, 0.7);
		printf("lpf: %.2lf\n", low_pass_filter_data);

	}
	return 0;
}
void input_data(int mask, int INDEX)
{
	printf("데이터를 입력하세요 ");
	scanf_s("%lf", &data[INDEX]);

	for (int j = 0; j < mask; j++)
	{
		printf("%.2lf ", data[j]);
	}


}
double low_pass_filter(double RECENT_DATA, double alpha)
{
	double x_lpf = 0.0;

	x_lpf = alpha * prev_averge + (1 - alpha) * RECENT_DATA;
	prev_averge = x_lpf;


	return x_lpf;
}


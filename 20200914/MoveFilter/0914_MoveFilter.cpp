#include<stdio.h>

double data[] = { 0, };
int index = 0;


int input_data_num();
int input_mask();
void input_data(int data_num);
void input_move_data(int mask, int index);
double AvgFilter(int data_num);
double MoveFilter(int mask);

int main(void)
{
	int data_num = 0, mask = 0, index = 0;
	float avgfilter = 0.0;
	float movefilter = 0.0;

	/*// AvgFilter
	data_num = input_data_num();
	input_data(data_num);
	avgfilter = AvgFilter(data_num);
	printf("평균필터를 이용한 avg : %.2lf", avgfilter);*/
	mask = input_mask();
	while (1)
	{

		input_move_data(mask,index);
		index++;
		if (index >= mask)
		{
			index = 0;
		}
		movefilter = MoveFilter(mask);
		printf("-------> 이동평균필터를 이용한 avg : %.2lf\n", movefilter);

	}

	return 0;
}

int input_mask()
{
	int mask = 0;
	printf("이동평균의 Mask 길이를 입력하세요 : ");
	scanf_s("%d", &mask);
	return mask;
}

int input_data_num()
{
	int data_num = 0;

	// AvgFilter
	printf("데이터 갯수를 입력하세요 : ");
	scanf_s("%d", &data_num);


	return data_num;
}
void input_data(int data_num)
{
	for (int i = 0; i < (data_num); i++)
	{
		printf("%d 번쨰 데이터를 입력해주세요 : ", i + 1);
		scanf_s("%lf", &data[i]);
		for (int j = 0; j < (data_num); j++)
		{
			printf("%.2lf ", data[j]);
		}
		printf("\n");

	}

}
void input_move_data(int mask, int index)
{
	double m_data = 0;

	scanf_s("%lf", &m_data);
	data[index] = m_data;
	
	for (int j = 0; j < mask; j++)
	{
		printf("%.2lf ", data[j]);
	}
	//printf("\n");

}

double AvgFilter(int data_num )
{
	double prev_average = 0.0, prev_sum = 0.0, average = 0.0, alpha=0.0;
	for (int i = 0; i < (data_num - 1); i++)
	{
		prev_sum += data[i];
	}
	prev_average = prev_sum / (data_num - 1);
	alpha = (data_num - 1.0) / (data_num+0.0);
	average = alpha * prev_average + (1 - alpha) * data[data_num-1];

	return average;
}
double MoveFilter(int mask)
{
	double sum = 0.0, movefilter = 0.0;
	for (int i = 0; i < mask; i++)
	{
		sum += data[i];
	}
	movefilter = sum / mask;

	return movefilter;
}
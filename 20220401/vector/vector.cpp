#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iostream>
#include <cstdlib>
#include <ctime>


typedef float real;

class Vector3
{
	public:
	real x;
	real y;
	real z;
	
	private:
	real pad;
};


Vector3 operator_add(Vector3 v_1, Vector3 v_2)
{
	Vector3 vt;
	vt.x = v_1.x+v_2.x;
	vt.y = v_1.y+v_2.y;
	vt.z = v_1.z+v_2.z;
	return vt;
}

Vector3 operator_sub(Vector3 v_1, Vector3 v_2)
{
	Vector3 vt;
	vt.x = v_1.x-v_2.x;
	vt.y = v_1.y-v_2.y;
	vt.z = v_1.z-v_2.z;
	return vt;
}
Vector3 unit_vector(Vector3 v)
{
	real n = 0.0;
	n = sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));
	v.x = v.x/n;
	v.y = v.y/n;
	v.y = v.z/n;
	return v;
}
class Matrix3
{
	public:
	real v_num[9];
	
	public:
	Matrix3(){}
	Matrix3(Vector3 v)
	{
		v_num[0] = v.x; v_num[1] = -1.0; v_num[2] = -1.0;
		v_num[3] = v.y; v_num[4] = -1.0; v_num[5] = -1.0;
		v_num[6] = v.z; v_num[7] = -1.0; v_num[8] = -1.0;
	}
	 
};

Matrix3 Trans_matrix(Matrix3 m)
{
	real temp;
	temp = m.v_num[3];
	m.v_num[3]= -1.0;
	m.v_num[1] = temp;
	temp = m.v_num[6];
	m.v_num[6]= -1.0;
	m.v_num[2] = temp;
	
	return m;
}
real inner_product(Matrix3 A, Matrix3 B)
{
	Matrix3 t_m = Trans_matrix(A);
	real result = A.v_num[0]*B.v_num[0]+A.v_num[3]*B.v_num[3]+A.v_num[6]*B.v_num[6];
	return result;
}


int main(void)
{
	srand(time(0));
	
	Vector3 v1;
	v1.x = rand()%10;
	v1.y = rand()%10;
	v1.z = rand()%10;
	
	printf("x1 : %.2lf   y1 : %.2lf   z1 : %.2lf\n", v1.x,v1.y,v1.z);
	Vector3 v2;
	v2.x = rand()%10;
	v2.y = rand()%10;
	v2.z = rand()%10;
	
	printf("x2 : %.2lf   y2 : %.2lf   z2 : %.2lf\n", v2.x,v2.y,v2.z);
	
	Vector3 add_result;
	
	add_result = operator_add(v1,v2);
	printf("벡터의 합 : x : %.2lf   y : %.2lf   z : %.2lf \n", add_result.x,add_result.y,add_result.z);
	
	Vector3 sub_result;
	sub_result = operator_sub(v1,v2);
	printf("벡터의 차 : x : %.2lf   y : %.2lf   z : %.2lf \n", sub_result.x,sub_result.y,sub_result.z);
	
	Vector3 unit_v1 = unit_vector(v1);
	Vector3 unit_v2 = unit_vector(v2);
	
	real inner_result =0.0;
	inner_result = inner_product(unit_v1,unit_v2);
	printf("벡터의 내적: %.2lf \n", inner_result);
}

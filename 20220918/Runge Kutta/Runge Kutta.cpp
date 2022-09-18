#include <stdio.h>
#include <math.h>

#define K 3.0
#define B 2.0
#define M 10.0
#define h 0.25



double Function(double velocity, double position);

double pre_Velocity = 1.0, pre_Position = 0.0; // V0, X0

typedef struct value
{
	double d_x;
	double d_v;
}values;

values calculate(double v_n, double x_n);


int main(void)
{
	values Kutta;

	double t0 = 0.0, t_f = 5.0;
	double N = (t_f - t0) / h;
	double Velocity = 0.0, Position = 0.0;

	for (double i = t0; i < N; i += h)
	{
		Kutta = calculate(pre_Velocity, pre_Position);
		Velocity = pre_Velocity + Kutta.d_v;
		Position = pre_Position + Kutta.d_x;

		pre_Velocity = Velocity;
		pre_Position = Position;

		printf("POSITION : %.5lf   VELOCITY : %.5lf \n", Position, Velocity);

	}
	return 0;
}
values calculate(double v_n, double x_n)
{
	values p;
	double Kx1=0.0, Kx2=0.0, Kx3=0.0, Kx4=0.0, Kv1=0.0, Kv2=0.0, Kv3=0.0, Kv4=0.0;
	double dx = 0.0 , dv = 0.0;

	//n =0
	Kx1 = v_n;
	Kv1 = Function(v_n, x_n);
	Kx2 = v_n + ((h / 2.0)*Kv1);
	Kv2 = Function(x_n + ((h / 2) * Kx1), v_n + ((h / 2.0) * Kv1));
	Kx3 = v_n + (h / 2.0) * Kv2;
	Kv3 = Function(x_n + ((h / 2.0) * Kx2), v_n + ((h / 2.0) * Kv2));
	Kx4 = v_n + (h * Kv3);
	Kv4 = Function(x_n + (h * Kx3), v_n + (h * Kv3));

	//printf("%.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf, %.5lf \n", Kx1, Kv1, Kx2, Kv2, Kx3, Kv3, Kx4, Kv4);

	p.d_x = (h / 6.0) * (Kx1 + (2.0 * Kx2) + (2.0 * Kx3) + Kx4);
	p.d_v = (h / 6.0) * (Kv1 + (2.0 * Kv2) + (2.0 * Kv3) + Kv4);
	//printf("d_x : %.5lf, d_v : %.5lf\n", p.d_x, p.d_v);
	return p;
}
double Function(double velocity, double position)
{
	double f = 0.0;
	f =  -(B / M) * velocity - (K / M) * position;
	return f;
}
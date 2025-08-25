#include <math.h>

f64 Sin(f64 x)
{
	while (x > PI)
		x -= 2 * PI;
	while (x < -PI)
		x += 2 * PI;

	f64 res = 0.0, term = x;
	s32 n = 1;

	while (term > 0.00001 || term < -0.00001)
	{
		res += term;
		n += 2;
		term = -term * x * x / (n * (n - 1));
	}

	return res;
}

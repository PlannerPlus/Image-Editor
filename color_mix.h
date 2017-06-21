#ifndef COLOR_MIX_H
#define COLOR_MIX_H 
//三维向量 
struct Vector3 {
	double v[3];
	Vector3()
	{
		for (int i = 0; i < 3; i++)
		{
			v[i] = 0.0;
		}
	}
	Vector3(double a0, double a1, double a2)
	{
		v[0] = a0; v[1] = a1; v[2] = a2;
	}
};
//较大较小函数 
double max(double a, double b)
{
	if (a < b) return b;
	return a;
}
double min(double a, double b)
{
	if (a < b) return a;
	return b;
}
#endif

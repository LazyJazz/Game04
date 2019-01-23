#include"Rand.h"

void InitRandomSystem()
{
	srand(time(NULL));
}

int Rand(int L, int R)
{
	return ((rand()<<15)|rand())%(R-L+1)+L;
}

double RandDouble(double L, double R)
{
	int l = L*10000.0, r = R*10000.0;
	return Rand(l, r) / 10000.0;
}

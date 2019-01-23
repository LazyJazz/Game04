#include "MathLib.h"

bool iszero(double a) { if (abs(a) <= EPS)return true; return false; }

double Dist(Line l, Vector v) { return (l.a*v.x + l.b*v.y) - l.c; }

Vector Dist(Vector v1, Vector v2, Vector p)
{
	if (iszero((v1 - v2).Dist()))return v1;
	Line l = Line(v1, v2);
	auto e1 = l + v1, e2 = l + v2;
	if (signal(e1.Check(v2)) != signal(e1.Check(p)))return v1;
	else if (signal(e2.Check(v1)) != signal(e2.Check(p)))return v2;
	return l && (l + p);
}
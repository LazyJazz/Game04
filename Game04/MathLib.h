#pragma once
#include<iostream>
#include<cmath>
#include<cstdio>
#include<vector>
#include<cstring>
#include<algorithm>
#define EPS 1e-10
#define PI 3.14159265358979323846264338327950288419716939937510
#define signal(x) ((x)<0?-1:((x)==0?0:1))
#define DEGREE(x) ((x)/180.0*PI)
using namespace std;
bool iszero(double a);
struct Vector
{
	double x, y;
	Vector(double x = 0.0, double y = 0.0)
	{
		this->x = x;
		this->y = y;
	}
	Vector operator + (const Vector &b) const { return Vector(x + b.x, y + b.y); }
	Vector operator +=(const Vector &b) { *this = *this + b; return *this; }
	Vector operator - (const Vector &b) const { return Vector(x - b.x, y - b.y); }
	Vector operator -=(const Vector &b) { *this = *this - b; return *this; }
	double operator * (const Vector &b) const { return x*b.x + y*b.y; }
	friend Vector operator * (const Vector &v, const double &d) { return Vector(v.x*d, v.y*d); }
	friend Vector operator * (const double &d, const Vector &v) { return Vector(v.x*d, v.y*d); }
	Vector operator *= (const double &b) { *this = *this*b; return *this; }
	double operator ^ (const Vector &b) const { return x*b.y - y*b.x; }
	Vector operator / (const double &d) const { return Vector(x / d, y / d); }
	Vector operator /= (const double &d) { *this = *this / d; return *this; }
	bool operator < (const Vector &b) const { return x<b.x ? 1 : (x == b.x ? (y<b.y) : 0); }
	double Dist() { return sqrt(x*x + y*y); }
	Vector Zoom(double r) { return Dist() <= 0.00000001 ? Vector(r, 0) : (*this / Dist()*r); }
	Vector Rotate(double d) {
		double D = atan2(x, y), l = Dist(); D += d;
		return *this = Vector(sin(D)*l, cos(D)*l);
	}
	Vector Vertical(){return Vector(y, -x);}
	void Print() { printf("(%lf,%lf)", x, y); }
};
struct Vector3D
{
	double x, y, z;
	Vector3D() { x = y = z = 0.0; }
	Vector3D(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3D operator + (const Vector3D &b) const { return Vector3D(x + b.x, y + b.y, z + b.z); }
	Vector3D operator +=(const Vector3D &b) { *this = *this + b; return *this; }
	Vector3D operator - (const Vector3D &b) const { return Vector3D(x - b.x, y - b.y, z - b.z); }
	Vector3D operator -=(const Vector3D &b) { *this = *this - b; return *this; }
	double operator * (const Vector3D &b) const { return x*b.x + y*b.y + z*b.z; }
	friend Vector3D operator * (const Vector3D &v, const double &d) { return Vector3D(v.x*d, v.y*d, v.z*d); }
	friend Vector3D operator * (const double &d, const Vector3D &v) { return Vector3D(v.x*d, v.y*d, v.z*d); }
	Vector3D operator *= (const double &b) { *this = *this*b; return *this; }
	Vector3D operator / (const double &d) const { return Vector3D(x / d, y / d, z / d); }
	Vector3D operator /= (const double &d) { *this = *this / d; return *this; }
	double Dist() { return sqrt(x*x + y*y + z*z); }
	Vector3D Zoom(double r) { return *this = *this / Dist()*r; }
	Vector3D Rotate(double dx, double dy, double dz) {
		double D, l;
		D = atan2(x, y); l = Vector(x, y).Dist(); D += dz;
		*this = Vector3D(sin(D)*l, cos(D)*l, z);
		D = atan2(y, z); l = Vector(y, z).Dist(); D += dx;
		*this = Vector3D(x, sin(D)*l, cos(D)*l);
		D = atan2(x, z); l = Vector(x, z).Dist(); D += dy;
		*this = Vector3D(sin(D)*l, y, cos(D)*l);
		return *this;
	}
};
struct Coordinate
{
	Vector3D x, y, z;
	Coordinate()
	{
		x = Vector3D(1.0, 0.0, 0.0);
		y = Vector3D(0.0, 1.0, 0.0);
		z = Vector3D(0.0, 0.0, 1.0);
	}
	Coordinate(double dx, double dy, double dz)
	{
		x = Vector3D(1.0, 0.0, 0.0);
		y = Vector3D(0.0, 1.0, 0.0);
		z = Vector3D(0.0, 0.0, 1.0);
		x.Rotate(dx, dy, dz);
		y.Rotate(dx, dy, dz);
		z.Rotate(dx, dy, dz);
	}
	Vector3D operator * (const Vector3D &b) const { return b.x*x + b.y*y + b.z*z; }
	Coordinate Rotate(double dx, double dy, double dz)
	{
		x.Rotate(dx, dy, dz);
		y.Rotate(dx, dy, dz);
		z.Rotate(dx, dy, dz);
	}
};
struct Line
{
	double a, b, c;//ax+by=c;
	Line(double a = 0.0, double b = 1.0, double c = 0.0) { this->a = a; this->b = b; this->c = c; double scale = 1.0 / sqrt(a*a + b*b); a *= scale, b *= scale, c *= scale; }
	Line(Vector v1, Vector v2) { v1 -= v2; a = v1.y; b = -v1.x; c = a*v2.x + b*v2.y; double scale = 1.0 / sqrt(a*a + b*b); a *= scale, b *= scale, c *= scale; }
	Vector operator && (const Line &l) const { if (iszero(a*l.b - b*l.a))return Vector(); return Vector((c*l.b - l.c*b) / (a*l.b - l.a*b), (c*l.a - l.c*a) / (b*l.a - a*l.b)); }
	Line operator | (Vector v) const { return Line(a, b, a*v.x + b*v.y); }
	Line operator + (Vector v) const { return Line(b, -a, 0) | v; }
	double Check(Vector v) { return v.x*a + v.y*b - c; }
	void FixDirection(Vector v) { if (Check(v) < 0.0)a = -a, b = -b, c = -c; }
	void Print() { printf("%lfx+%lfy=%lf", a, b, c); }
};
double Dist(Line l, Vector v);
struct Circle
{
	double x, y, r;
	Circle(double x = 0.0, double y = 0.0, double r = 0.0) { this->x = x; this->y = y; this->r = r; }
	Vector Center() { return Vector(x, y); }
	void Print() { printf("(%lf,%lf,%lf)", x, y, r); }
};
Vector Dist(Vector v1, Vector v2, Vector p);


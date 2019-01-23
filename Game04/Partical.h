#pragma once
#include"Draw.h"
#include"Rand.h"
#define PARTICAL 1048576
struct Partical
{
	double x, y, speedx, speedy, life, lim;
	int color1, color2;
	void(*print)(Partical&);
	void(*update)(Partical&);
};

void AddPartical(double x, double y, double speedx, double speedy, double lim, int color1, int color2,
	void(*printfunc)(Partical&),
	void(*updatefunc)(Partical&));

void UpdateParticals();

void PrintParticals();

void SmokeParticalUpdate(Partical&part);

void SmokeParticalPrint(Partical& part);

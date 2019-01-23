#pragma once
#include<Windows.h>
#include"MathLib.h"
#include"Draw.h"
#include<ctime>
#include<queue>

struct CSNAPSHOT
{
	int time;
	bool press[256];
	Vector mousepos;
};
void TakeSnapshot(CSNAPSHOT &snapshot);

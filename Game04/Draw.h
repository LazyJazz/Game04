#pragma once
#include<graphics.h>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<iostream>
#include"MathLib.h"
#include"functions.h"
#include<imm.h>
#include"resource.h"
#pragma comment(lib,"imm32.lib")
using namespace std;

void CreateWin();

char * TcharToChar(const TCHAR * tchar);

TCHAR * CharToTchar(const char * _char);

void PutString(const char *str, int x, int y, int color = -1);

void PutStringH(const char *str, int x, int y, int height, int color = -1);

void SetTextStyle(int height, const char * font);

void MixPixel(int p, int color, double a);

int MixColor(int color1, int color2, double a);

void PutStringMid(const char * str, int x, int y);

void PutStringMidH(const char * str, int x, int y, int height, int color);

void PrintCircle(double x, double y, double r, int color);

void PrintRectangle(double x1, double y1, double x2, double y2, int color);

void PrintLine(double x1, double y1, double x2, double y2, int color, double width = 1.0);

void PrintHalo(double x, double y, double r, int color);

void PrintCircle(double x, double y, double r, int color, double alpha);

void PrintRectangle(double x1, double y1, double x2, double y2, int color, double alpha);

void Transform(double &x, double &y);

void RevTransform(double &x, double &y);

double MinDist(double x, double y, double x1, double y1, double x2, double y2);

void SetCam(double camx, double camy);

void PrintRectangleScale(double x1, double y1, double x2, double y2, int color, double alpha = 1.0);

void PrintCircleScale(double x, double y, double r, int color, double alpha = 1.0);

void PrintLineScale(double x1, double y1, double x2, double y2, int color, double width = 1.0);

void PrintHaloScale(double x, double y, double r, int color);

void PrintFan(double x, double y, double ds, double dp, double r1, double r2, int color, double alpha = 1.0);

void PrintFanScale(double x, double y, double ds, double dp, double r1, double r2, int color, double alpha = 1.0);

double GetScale();

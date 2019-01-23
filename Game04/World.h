#pragma once
#include<cmath>
#include"Rand.h"
#include"Draw.h"
#include"Partical.h"
#include"Control.h"
#include"SocketLib.h"
#define MAXB 1024
#define MAXP 64
#define MAXR 1024
#define PLAYERX 12
#define PLAYERY 18
#define BULLET 1048576
#define TRACKCOE 0.99
struct Player
{
	double x, y;
	double speedx, speedy;
	double health;
	int state;//-1:dead 0:normal 
	double energy;
	double cont;
	int type;
	double cd[10];
	CSNAPSHOT ctrl;
	char name[128];
	int side;
};

struct Bullet
{
	double x, y, speedx, speedy, life, lim;
	int from;
	int type;
};

int GetFrame();

Vector GetPlayerPos(int id);

void AddBlock(double x1, double y1, double x2, double y2);

void PrintWorld(double camx, double camy, int id);

void UpdateWorld(int ms, int id);

void Control(int id, CSNAPSHOT ctrl);

void Control(string name, CSNAPSHOT ctrl);

int CheckPlayerId(string name);

void SetRebornPoint(double x1, double y1, double x2, double y2);

void SetBase(double x1, double y1, double x2, double y2);

int AddPlayer(int side, string name);

void AddResource(double x, double y);

void SetBorder(double x1, double y1, double x2, double y2);

void GetBorder(double &x1, double &y1, double &x2, double &y2);

void PackWorldInfo(SendBag &sendbag);

void UnpackWorldInfo(Message & message, int id);

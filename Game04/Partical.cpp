#include"Partical.h"
Partical particals[PARTICAL];
int cnt_partical=0;

void AddPartical(double x, double y, double speedx, double speedy, double lim, int color1, int color2,
	void(*printfunc)(Partical&),
	void(*updatefunc)(Partical&))
{
	particals[cnt_partical].x = x;
	particals[cnt_partical].y = y;
	particals[cnt_partical].speedx = speedx;
	particals[cnt_partical].speedy = speedy;
	particals[cnt_partical].lim = lim;
	particals[cnt_partical].life = 0;
	particals[cnt_partical].color1 = color1;
	particals[cnt_partical].color2 = color2;
	particals[cnt_partical].print = printfunc;
	particals[cnt_partical].update = updatefunc;
	cnt_partical++;
}

void UpdateParticals()
{
	int new_cnt_p=0;
	for (int i = 0; i < cnt_partical; i++)
	{
		particals[i].life += 0.001;
		particals[i].update(particals[i]);
		if (particals[i].life <= particals[i].lim)
		{
			particals[new_cnt_p++] = particals[i];
		}
	}
	cnt_partical = new_cnt_p;
}

void PrintParticals()
{
	for (int i = 0; i < cnt_partical; i++)
	{
		particals[i].print(particals[i]);
	}
}

void SmokeParticalUpdate(Partical&part)
{
	part.x += RandDouble(-part.speedy, part.speedy)*0.001;
	part.y += RandDouble(-part.speedy, part.speedy)*0.001;
}

void SmokeParticalPrint(Partical& part)
{
	double alpha = part.lim*0.5 - part.life; alpha = abs(alpha);
	alpha = part.lim*0.5 - alpha;
	PrintCircleScale(part.x, part.y, part.speedx*(-part.life / part.lim + 1.5), part.color1, alpha*0.9+0.1);
}

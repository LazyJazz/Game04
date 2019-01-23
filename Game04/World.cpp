#include"World.h"

double block[MAXB][2][2];
int cntb = 0;

double rebornpos[2][2], basepos[2][2];
double damageprop[2];
bool baseprotect[2];

double resources[MAXR][2], d = 0.0;
int cntd = 0;
int frame = 0;

double border[2][2];

Bullet bullets[BULLET];
int cnt_bullet = 0;

Player players[MAXP];
int cntp = 0;

int MES_from[10], MES_to[10], MES_time[10], cnt_MES = 0;

void DelMES()
{
	for (int i = 1; i < cnt_MES; i++)MES_from[i - 1] = MES_from[i], MES_to[i - 1] = MES_to[i], MES_time[i - 1] = MES_time[i];
	cnt_MES--;
}
void AddMES(int from, int to)
{
	if (cnt_MES == 10)DelMES();
	MES_from[cnt_MES] = from;
	MES_to[cnt_MES] = to;
	MES_time[cnt_MES] = 7000;
	cnt_MES++;
}
void UpdMES()
{
	for (int i = 0; i < cnt_MES; i++)MES_time[i]--;
	while (cnt_MES && !MES_time[0])DelMES();
}
void PrintMES(int side)
{
	int height = getheight()*0.03, sy = height, sx = getwidth()*0.98, x, tx1, tx2, color1, color2;
	SetTextStyle(height, "Î¢ÈíÑÅºÚ");
	for (int i = 0; i < cnt_MES; i++)
	{
		tx1 = TextWidth(players[MES_from[i]].name);
		tx2 = TextWidth(players[MES_to[i]].name);
		if (players[MES_from[i]].side == side)color1 = 0xFF0000;
		else color1 = 0xFF;
		if (players[MES_to[i]].side == side)color2 = 0xFF0000;
		else color2 = 0xFF;
		x = sx - tx1 - tx2 - height;
		PrintRectangle(x, sy, sx, sy + height-1, 0, 0.1);
		PutString(players[MES_from[i]].name, x, sy, color1);
		x += tx1;
		for (int i = 0, range = height*0.8 - height*0.2, mid = height >> 1, L = height*0.2; i < range; i++)
		{
			int h = height*0.3*(range - i) / range;
			for (int j = -h; j <= h; j++)
			{
				putpixel(x + L + i, sy + mid + j, 0xFFFFFF);
			}
		}
		x += height;
		PutString(players[MES_to[i]].name, x, sy, color2);
		sy += height;
		
	}
}

int GetFrame()
{
	return frame;
}
void MeteoriteUpdate(Bullet&bullet, int id, int num);
void MeteoritePrint(Bullet&bullet, int id);

void RifleUpdate(Bullet&bullet, int id, int num);
void RiflePrint(Bullet&bullet, int id);

void ExplosionUpdate(Bullet&bullet, int id, int num);
void ExplosionPrint(Bullet&bullet, int id);

void RocketUpdate(Bullet&bullet, int id, int num);
void RocketPrint(Bullet&bullet, int id);

void GravityBallUpdate(Bullet&bullet, int id, int num);
void GravityBallPrint(Bullet&bullet, int id);

void ManaBurnUpdate(Bullet&bullet, int id, int num);
void ManaBurnPrint(Bullet&bullet, int id);

void SniperUpdate(Bullet&bullet, int id, int num);
void SniperPrint(Bullet&bullet, int id);

void(*bulletprintfunc[128])(Bullet&, int id) = { RocketPrint,ExplosionPrint,GravityBallPrint,ManaBurnPrint,RiflePrint,MeteoritePrint,SniperPrint };
void(*bulletupdatefunc[128])(Bullet&, int, int) = { RocketUpdate,ExplosionUpdate,GravityBallUpdate,ManaBurnUpdate,RifleUpdate,MeteoriteUpdate,SniperUpdate };

void Normal_Rocket(int id);
void Normal_Rifle(int id);
void Normal_Sniper(int id);

void Skill_GravityBall(int id);
void GravityBallIcon(int x, int y, int C);
void Skill_Meteorite(int id);
void MeteoriteIcon(int x, int y, int C);

void Skill_Sting(int id);
void StingIcon(int x, int y, int C);
void Skill_Invisible(int id);
void InvisibleIcon(int x, int y, int C);

void Ultimate_Manaburn(int id);
void ManaburnIcon(int x, int y, int C);
void Ultimate_Crazyshoot(int id);
void CrazyshootIcon(int x, int y, int C);
void Ultimate_Barrage(int id);
void BarrageIcon(int x,int y,int C);

void Contskill_Sacrifice(int id);
void Contskill_Jetpack(int id);

void(*NormalSkill[128])(int) = { Normal_Rocket,Normal_Rifle };

void(*EpicSkill[128])(int) = { Skill_Meteorite,Skill_GravityBall};
void(*EpicSkillIcon[128])(int, int, int) = { MeteoriteIcon,GravityBallIcon };

void(*LegendSkill[128])(int) = { Ultimate_Barrage,Ultimate_Crazyshoot };
void(*LegendSkillIcon[128])(int, int, int) = { BarrageIcon,CrazyshootIcon };

void(*RareSkill[128])(int) = { Skill_Sting,Skill_Invisible };
void(*RareSkillIcon[128])(int, int, int) = { StingIcon,InvisibleIcon };

void(*ContSkill[128])(int) = { Contskill_Jetpack,Contskill_Sacrifice };

void GravityBallIcon(int x, int y, int C)
{
	PrintCircle(x + C*0.5, y + C*0.5, C*0.1, 0xFFFFFF);
	for (int i = 0; i < 10; i++)
	{
		double d = frame*0.001 + i*0.01666, px, py;
		d *= 2 * PI;
		px = sin(d)*0.25*C; py = cos(d)*0.25*C;
		PrintCircle(x + C*0.5 + px, y + C*0.5 + py, C*0.002*i, 0xFFFFFF);

		d += PI * 2 / 3;
		px = sin(d)*0.25*C; py = cos(d)*0.25*C;
		PrintCircle(x + C*0.5 + px, y + C*0.5 + py, C*0.002*i, 0xFFFFFF);

		d += PI * 2 / 3;
		px = sin(d)*0.25*C; py = cos(d)*0.25*C;
		PrintCircle(x + C*0.5 + px, y + C*0.5 + py, C*0.002*i, 0xFFFFFF);
	}
}

void MeteoriteIcon(int x, int y, int C)
{
	PrintCircle(x + C*0.5, y + C*0.6, C*0.2, 0xFFFFFF);
	for (double my = C*0.1; my < C*0.9; my+=0.5)
	{
		double mx = cbrt(1.0 - (my - C*0.1)*1.25 / C);
		if (((int)my + frame/ 5) % C < (C >> 1))
		{
			PrintCircle(x + (1 - mx)*C*0.5, y + my, C*0.03, 0xFFFFFF);
			PrintCircle(x + (1 + mx)*C*0.5, y + my, C*0.03, 0xFFFFFF);
		}
	}
}

void StingIcon(int x, int y, int C)
{
	for (int i = 0; i < C; i++)
	{
		int px = (frame / 5 + i) % C;
		if (px * 2 < C)PrintCircle(x + i, y + C*(0.5 + 0.3*pow((C - i) / (double)C, 0.3333333333333)), 0.5, 0xFFFFFF), PrintCircle(x + i, y + C*(0.5 - 0.3*pow((C - i) / (double)C, 0.3333333333333)), 0.5, 0xFFFFFF);
		if (i < C*0.8)PrintCircle(x + i, y + C*0.5, i*0.1, 0xFFFFFF);
	}
}

void InvisibleIcon(int x, int y, int C)
{
	double a; int f = frame % 3000;
	if (f < 300)a = f / 300.0;
	else if (f < 750)a = 1.0;
	else if (f < 2250)a = 1.0 - (f - 750) / 1500.0;
	else a = 0.0;
	PrintRectangle(x + C*0.3, y + C*0.1, x + C*0.7, y + C*0.9, 0xFFFFFF, a);
}

void BarrageIcon(int x, int y, int C)
{
	x -= C;
	y -= C;
	C *= 2;
	PrintRectangle(x + C*0.1, y + C*0.3, x + C*0.15, y + C*0.7, 0xFFFFFF);
	PrintRectangle(x + C*0.18, y + C*0.3, x + C*0.75, y + C*0.7, 0xFFFFFF);
	PrintCircle(x + C*0.75, y + C*0.5, C*0.2, 0xFFFFFF);
}

void ManaburnIcon(int x, int y, int C)
{
	for (double d = 0.0; d < 180.0; d += 0.5)
	{
		double rd = DEGREE(2 * d), r;
		if (d < 180.0)r = C*0.8*sin((d / 180.0)*PI);
		else r = C*0.8*sin(((360.0 - d) / 180.0)*PI);
		PrintCircle(x + r*sin(rd), y + r*cos(rd), C*0.01, 0xFFFFFF);
		rd += DEGREE(72.0);
		PrintCircle(x + r*sin(rd), y + r*cos(rd), C*0.01, 0xFFFFFF);
		rd += DEGREE(72.0);
		PrintCircle(x + r*sin(rd), y + r*cos(rd), C*0.01, 0xFFFFFF);
		rd += DEGREE(72.0);
		PrintCircle(x + r*sin(rd), y + r*cos(rd), C*0.01, 0xFFFFFF);
		rd += DEGREE(72.0);
		PrintCircle(x + r*sin(rd), y + r*cos(rd), C*0.01, 0xFFFFFF);
	}
}

void CrazyshootIcon(int x, int y, int C)
{
	x -= C;
	y -= C;
	C *= 2;
	for (int i = 0; i < 8; i++)
	{
		PrintRectangle(x + C*0.2, y + C*0.1*(i + 1.2), x + C*0.23, y + C*0.1*(i + 1.8), 0xFFFFFF);
		PrintRectangle(x + C*0.25, y + C*0.1*(i + 1.2), x + C*0.5, y + C*0.1*(i + 1.8), 0xFFFFFF);
		for (int j = 0; j < C*0.32; j++)
		{
			double s = j / (C*0.32); s *= s*s*s;
			PrintCircle(x + C*0.5 + j, y + C*0.1*(i + 1.5), C*0.027*(1 - s), 0xFFFFFF);
		}
	}
	//PrintRectangle();
}

void SetBorder(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);
	border[0][0] = x1;
	border[0][1] = x2;
	border[1][0] = y1;
	border[1][1] = y2;
}

void GetBorder(double &x1, double &y1, double &x2, double &y2)
{
	x1 = border[0][0];
	x2 = border[0][1];
	y1 = border[1][0];
	y2 = border[1][1];
}

bool InBorder(double x, double y)
{
	if (x < border[0][0])return false;
	else if (x > border[0][1])return false;
	else if (y < border[1][0])return false;
	else if (y > border[1][1])return false;
	else return true;
}
void AddBullet(int from, double x, double y, double speedx, double speedy, double lim, int type)
{
	bullets[cnt_bullet].x = x;
	bullets[cnt_bullet].y = y;
	bullets[cnt_bullet].speedx = speedx;
	bullets[cnt_bullet].speedy = speedy;
	bullets[cnt_bullet].lim = lim;
	bullets[cnt_bullet].life = 0;
	bullets[cnt_bullet].type = type;
	bullets[cnt_bullet].from = from;
	cnt_bullet++;
}
void UpdateBullets(int id)
{
	int ncb = 0;
	for (int i = 0; i < cnt_bullet; i++)
	{
		if (bullets[i].life > bullets[i].lim)continue;
		bulletupdatefunc[bullets[i].type](bullets[i], id, i);
		if (bullets[i].life <= bullets[i].lim&&InBorder(bullets[i].x, bullets[i].y))
			bullets[ncb++] = bullets[i];
	}
	cnt_bullet = ncb;
}
void PrintBullets(int id)
{
	for (int i = 0; i < cnt_bullet; i++)bulletprintfunc[bullets[i].type](bullets[i], id);
}
int cntr = 0;

void SetRebornPoint(double x1, double y1, double x2, double y2)
{
	rebornpos[0][0] = x1;
	rebornpos[0][1] = y1;
	rebornpos[1][0] = x2;
	rebornpos[1][1] = y2;
}

void SetBase(double x1, double y1, double x2, double y2)
{
	basepos[0][0] = x1;
	basepos[0][1] = y1;
	basepos[1][0] = x2;
	basepos[1][1] = y2;
}

void AddBlock(double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);
	block[cntb][0][0] = x1;
	block[cntb][0][1] = x2;
	block[cntb][1][0] = y1;
	block[cntb][1][1] = y2;
	cntb++;
	block[cntb][0][0] = -x2;
	block[cntb][0][1] = -x1;
	block[cntb][1][0] = y1;
	block[cntb][1][1] = y2;
	cntb++;
}

void AddResource(double x, double y)
{
	resources[cntr][0] = x;
	resources[cntr][1] = y;
	cntr++;
}
struct ResourcePart
{
	double x, y, z; int type, color;
	bool operator < (const ResourcePart &b) const
	{
		return z < b.z;
	}
}RP[4];
void MarkParticalUpdate(Partical&)
{
	;
}
void MarkParticalPrint(Partical& part)
{
	double alpha = 1.0 - (part.life / part.lim);
	PrintCircleScale(part.x, part.y, part.speedx*alpha, MixColor(part.color1, part.color2, part.life / part.lim), alpha);
}
void DirtParticalUpdate(Partical& part)
{
	part.speedy -= 2.0;
	part.x += part.speedx*0.001;
	part.y += part.speedy*0.001;
	for (int i = 0; i < cntb; i++)if (part.x >= block[i][0][0] && part.x <= block[i][0][1] && part.y >= block[i][1][0] && part.y <= block[i][1][1])part.life = part.lim + 1;
}
void DirtParticalPrint(Partical& part)
{
	double alpha = 1.0 - part.life / part.lim;
	PrintCircleScale(part.x, part.y, part.color2*0.5, part.color1, alpha);
}
void UpdateResources()
{
	d += DEGREE(360.0)*0.001;
	while (d >= 2 * PI)d -= 2 * PI;
	cntd++;
	if (cntd == 5)
	{
		cntd = 0;
		double x, y, z, nx, ny;
		RP[0].x = RP[0].y = RP[0].z = 0.0; RP[0].type = 0;
		d += 1.0 / 3.0*PI;
		x = sin(d)*50.0; z = cos(d)*50.0; y = z*-0.2;
		RP[1].x = x; RP[1].y = y; RP[1].z = z; RP[1].type = 1; RP[1].color = 0xFF0000;
		d += 1.0 / 3.0*PI;
		x = sin(d)*50.0; z = cos(d)*50.0; y = z*-0.2;
		nx = cos(2.0 / 3.0*PI)*x + sin(2.0 / 3.0*PI)*y;
		ny = cos(2.0 / 3.0*PI)*y - sin(2.0 / 3.0*PI)*x;
		x = nx;
		y = ny;
		RP[2].x = x; RP[2].y = y; RP[2].z = z; RP[2].type = 1; RP[2].color = 0xFF00;
		d += 1.0 / 3.0*PI;
		x = sin(d)*50.0; z = cos(d)*50.0; y = z*-0.2;
		nx = cos(4.0 / 3.0*PI)*x + sin(4.0 / 3.0*PI)*y;
		ny = cos(4.0 / 3.0*PI)*y - sin(4.0 / 3.0*PI)*x;
		x = nx;
		y = ny;
		RP[3].x = x; RP[3].y = y; RP[3].z = z; RP[3].type = 1; RP[3].color = 0xFF;
		d -= PI;
		for (int i = 0; i < 4; i++)
		{
			if (RP[i].type)
				if (RP[i].z > 0.0 || Vector(RP[i].x, RP[i].y).Dist() > 25.0)
					for (int j = 0; j < cntr; j++)
					{
						AddPartical(resources[j][0] + RP[i].x, resources[j][1] + RP[i].y,
							1.0, 0.0, 0.5, RP[i].color, RP[i].color|0x777777,
							MarkParticalPrint, MarkParticalUpdate
						);
					}
		}
	}
	/*//*/
	sort(RP, RP + 4);
}

void PrintResources()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < cntr; j++)
		{
			/*if (RP[i].type)
			{
				//PrintCircleScale(resources[j][0] + RP[i].x, resources[j][1] + RP[i].y, 5.0, 0x0000FF);
			}
			else
				PrintCircleScale(resources[j][0], resources[j][1], 25.0, 0x0088FF,0.6);
			//*/
			if (!RP[i].type)
				PrintCircleScale(resources[j][0], resources[j][1], 25.0, 0x0088FF, 0.6);
		}
	}
}


void AddEnergy(int id, double en)
{
	if (players[id].state == -1)en = 0.0;
	players[id].energy += en;
	players[id].energy = min(players[id].energy, 10000.0);
	players[id].energy = max(players[id].energy, 0.0);
}
void AddHealth(int id, double bl, int from)
{
	if (players[id].state == 1)bl = max(bl, 0);
	bl = max(bl, -players[id].health - 1.0);
	players[id].health += bl;
	players[id].health = min(players[id].health, 100.0);
	players[id].health = max(players[id].health, 0.0);
	if (bl < 0.0&&id != from)
	{
		AddEnergy(from, bl*-10.0);
	}
}

void KillPlayer(int id, int from)
{
	players[id].state = -1;
	players[id].cont = 8.0;
	AddMES(from, id);
}
int AddPlayer(int side, string name)
{
	players[cntp].x = rebornpos[side][0];
	players[cntp].y = rebornpos[side][1];
	players[cntp].speedx = 0.0;
	players[cntp].speedy = 0.0;
	players[cntp].side = side;
	players[cntp].energy = 0.0;
	players[cntp].health = 100;
	players[cntp].cont = 0.0;
	players[cntp].type = rand() % 2;
	memset(players[cntp].cd,0,sizeof(players[cntp].cd));
	memset(players[cntp].name, 0, sizeof(players[cntp].name));
	sprintf(players[cntp].name, "%s", name.c_str());
	return cntp++;
}

Vector GetPlayerPos(int id)
{
	return Vector(players[id].x, players[id].y);
}

void Control(int id, CSNAPSHOT ctrl)
{
	players[id].ctrl = ctrl;
}

void Control(string name, CSNAPSHOT ctrl)
{
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].name == name)
		{
			players[i].ctrl = ctrl;
			break;
		}
	}
}

int CheckPlayerId(string name)
{
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].name == name)return i;
	}
	return -1;
}


void LightspotParticalUpdate(Partical& part)
{
	part.speedy += 50.0*0.001;
	part.y += part.speedy*0.001;
}

void LightspotParticalPrint(Partical& part)
{
	double alpha = part.lim*0.5 - part.life; alpha = abs(alpha);
	alpha = part.lim*0.5 - alpha;
	PrintCircleScale(part.x, part.y, part.speedx, part.color1, alpha);
}

void FireParticalUpdate(Partical &part)
{
	part.speedx += RandDouble(-200.0, 200.0)*0.001;
	part.speedy += RandDouble(1000.0, 1800.0)*0.001;
	part.speedx *= 0.98,part.speedy *= 0.98;
	part.x += part.speedx*0.001;
	part.y += part.speedy*0.001;
}

void FireParticalPrint(Partical &part)
{
	double alpha, a;
	alpha = (part.lim*0.5 - abs(part.lim*0.5 - part.life))/ part.lim*0.5;
	a = (part.life / part.lim);
	PrintCircleScale(part.x, part.y, 0.5, MixColor(part.color1, part.color2, a),1.0-a);
}
void PrintPlayerStateblank(int id)
{
	double x = getwidth()*0.2, y = getheight()*0.025, sx = getwidth()*0.05, sy = getheight()*0.9 + 3, percent;
	int color;
	char str[128];
	sprintf(str, "%.0lf%%", players[id].energy*0.01);
	percent = players[id].energy*0.0001;
	PrintFan(getwidth()*0.5, getheight()*0.9, PI - percent * 2 * PI, percent * 2 * PI, getheight()*0.05, getheight()*0.075, 0x88FF, 0.7);
	PrintCircle(getwidth()*0.5, getheight()*0.9, getheight()*0.045, 0, 0.3);
	if(percent<1.0)PrintFan(getwidth()*0.5, getheight()*0.9, PI, 2 * PI - percent * 2 * PI, getheight()*0.05, getheight()*0.075, 0xFF7700, 0.7), PutStringMidH(str, getwidth()*0.5, getheight()*0.9, getheight()*0.05, 0xFFFFFF);
	else LegendSkillIcon[players[id].type](getwidth()*0.5, getheight()*0.9, getheight()*0.045);// , PutStringMidH("Ready£¡", getwidth()*0.5, getheight()*0.9, getheight()*0.03, 0x88FF);

	if (players[id].state == -1)
	{
		sprintf(str, "Reborn:%.1lfs", players[id].cont);
		percent = players[id].cont * 0.05;
		PutStringMidH(str, getwidth()*0.5, getheight()*0.5, getheight()*0.1, 0xFF);
		PrintLine(getwidth()*(0.5 - percent), getheight()*0.4, getwidth()*(0.5 + percent), getheight()*0.4, 0xFF0000, 1.0);
		PrintLine(getwidth()*(0.5 - percent), getheight()*0.6, getwidth()*(0.5 + percent), getheight()*0.6, 0xFF0000, 1.0);
	}
	int C = getheight()*0.05;
	x = getwidth() - C * 4, y = getheight() - 2 * C;
	PrintRectangle(x, y, x + C, y + C, 0, 0.3);
	if (players[id].cd[1] == 0.0)
	{
		EpicSkillIcon[players[id].type](x, y, C);
	}
	else sprintf(str, "%.0lf", players[id].cd[1] + 0.5), PutStringMidH(str, x + C*0.5, y + C*0.5, C, 0xFFFFFF);
	x += C*1.5;
	PrintRectangle(x, y, x + C, y + C, 0, 0.3);
	if (players[id].cd[2] == 0.0)
	{
		RareSkillIcon[players[id].type](x, y, C);
	}
	else sprintf(str, "%.0lf", players[id].cd[2] + 0.5), PutStringMidH(str, x + C*0.5, y + C*0.5, C, 0xFFFFFF);
}
double da = 0.0, rend_prop;
Vector3D basecore[12];
void PrintAperture(double x,double y,double ds, double dp, double r1, double r2, int color, double alpha)
{
	PrintFanScale(x, y, ds, dp*rend_prop, r1, r2, color, alpha);
}
void PrintBase(double x, double y, int color)
{
	PrintAperture(x, y, da, DEGREE(60.0), 45.0, 50.0, color, 0.3);
	PrintAperture(x, y, -da * 2, DEGREE(90.0), 51.0, 56.0, color, 0.4);
	PrintAperture(x, y, da * -5, DEGREE(45.0), 39.0, 44.0, color, 0.1);
	PrintAperture(x, y, da * 5, DEGREE(45.0), 39.0, 44.0, color, 0.1);
	PrintAperture(x, y, da * -5 + DEGREE(45.0)*rend_prop+DEGREE(5.0), DEGREE(45.0), 39.0, 44.0, color, 0.1);
	PrintAperture(x, y, da * 4, DEGREE(120.0), 58.0, 65.0, color, 0.1);
	PrintAperture(x, y, -da, DEGREE(45.0), 58.0, 65.0, color, 0.4);
	PrintAperture(x, y, da * -2 + DEGREE(200.0), DEGREE(60.0), 66.0, 70.0, color, 0.2);
	PrintAperture(x, y, da * -2 + DEGREE(65.0), DEGREE(90.0), 66.0, 70.0, color, 0.2);
	PrintAperture(x, y, da * 10, 1.5 * PI, 57, 57, color, 0.4);
	PrintAperture(x, y, da * -10, 2 * PI, 26, 26, color, 0.4);
	PrintAperture(x, y, da * 5 + DEGREE(36.0), 0.6 * PI, 26, 28, color, 0.4);
	PrintCircleScale(x, y, 25.0, color, 0.3*rend_prop);
	for (int i = 0; i < 10; i++)
	{
		int link = (i + 1) % 10;
		PrintLineScale(basecore[i].x + x, basecore[i].y + y, basecore[link].x + x, basecore[link].y + y, color, 0.1);
		link = (i + 2) % 10;
		PrintLineScale(basecore[i].x + x, basecore[i].y + y, basecore[link].x + x, basecore[link].y + y, color, 0.1);
	}
	for (int i = 0; i < 10; i += 2)
	{
		PrintLineScale(basecore[i].x + x, basecore[i].y + y, basecore[10].x + x, basecore[10].y + y, color, 0.1);
		PrintLineScale(basecore[i + 1].x + x, basecore[i + 1].y + y, basecore[11].x + x, basecore[11].y + y, color, 0.1);
	}
	//double x1 = x - 100.0, y1 = y + 90.0, x2 = 200.0*(blood*0.0002);
	//if (blood)PrintRectangleScale(x1, y1, x1 + x2, y1 + 10.0, MixColor(0xFF0000, 0xFF00, blood*0.0002), 0.5);
}
void PrintBases(int side)
{
	rend_prop = 1.0 - damageprop[side];
	rend_prop = 0.3 + rend_prop*0.7;
	PrintBase(basepos[side][0], basepos[side][1], 0xFF);
	rend_prop = 1.0 - damageprop[side ^ 1];
	rend_prop = 0.3 + rend_prop*0.7;
	PrintBase(basepos[side ^ 1][0], basepos[side ^ 1][1], 0xFF0000);
}
void UpdateBases()
{
	baseprotect[0] = false;
	baseprotect[1] = false;
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].state == -1)continue;
		if (Vector(players[i].x - basepos[players[i].side][0], players[i].y - basepos[players[i].side][1]).Dist() <= 150.0)baseprotect[players[i].side] = 1;
	}
	da += DEGREE(360.0)*0.0001;
	for (int i = 0; i < 2; i++)
	{
		if (baseprotect[i])damageprop[i] *= 0.995;
		else damageprop[i] = 1.0 - (1.0 - damageprop[i])*0.995;
	}
	double length = (Vector(0.0, 1.0) - Vector(sin(0.2*PI), cos(0.2*PI))).Dist(),
		length2 = (Vector(0.0, 1.0) - Vector(sin(0.4*PI), cos(0.4*PI))).Dist(),
		height = sqrt(length2*length2 - length*length)*0.5;
	Coordinate crd(da * 0.5, da * 0.7, da * 0.3);
	for (int i = 0; i < 10; i++)
	{
		basecore[i] = Vector3D(sin(i / 5.0*PI), cos(i / 5.0*PI), height);
		basecore[i] *= 15;
		basecore[i] = crd*basecore[i];
		height = -height;
	}
	basecore[10] = Vector3D(0.0, 0.0, height + sqrt(length2*length2 - 1.0));
	basecore[11] = Vector3D(0.0, 0.0, -height - sqrt(length2*length2 - 1.0));
	basecore[10] *= 15;
	basecore[10] = crd*basecore[10];
	basecore[11] *= 15;
	basecore[11] = crd*basecore[11];
}

void PrintWorld(double camx, double camy, int id)
{
	SetCam(camx, camy);
	double x1, y1, x2, y2;
	x1 = rebornpos[players[id].side][0] - PLAYERX;
	y1 = rebornpos[players[id].side][1] - PLAYERY;
	x2 = rebornpos[players[id].side][0] + PLAYERX;
	y2 = rebornpos[players[id].side][1] + PLAYERY;
	PrintCircleScale(rebornpos[1][0], rebornpos[1][1], 30.0, 0xFFFF00, 0.3);
	PrintCircleScale(rebornpos[0][0], rebornpos[0][1], 30.0, 0xFFFF00, 0.3);
	PrintRectangleScale(x1, y1, x2, y2, 0xB3);
	x1 = rebornpos[players[id].side^1][0] - PLAYERX;
	y1 = rebornpos[players[id].side^1][1] - PLAYERY;
	x2 = rebornpos[players[id].side^1][0] + PLAYERX;
	y2 = rebornpos[players[id].side^1][1] + PLAYERY;
	PrintRectangleScale(x1, y1, x2, y2, 0xB30000);
	for (int i = 0; i < cntb; i++)
	{
		x1 = block[i][0][0];
		y1 = block[i][1][0];
		x2 = block[i][0][1];
		y2 = block[i][1][1];
		PrintRectangleScale(x1, y1, x2, y2, 0x0);
	}
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].state == -1 || (players[i].state == 3 && players[i].side != players[id].side))continue;
		x1 = players[i].x - PLAYERX;
		y1 = players[i].y - PLAYERY;
		x2 = players[i].x + PLAYERX;
		y2 = players[i].y + PLAYERY;
		int color;
		if (i == id)color = 0x88FF88;
		else if (players[id].side == players[i].side)color = 0x8888FF;
		else color = 0xFF8888;
		if (players[i].state == 0 || players[i].state == 4)PrintRectangleScale(x1, y1, x2, y2, color);
		else if (players[i].state == 1 || players[i].state == 3)PrintRectangleScale(x1, y1, x2, y2, color, 0.5);
		y1 = players[i].y + PLAYERY + 5;
		x1 = players[i].x - PLAYERY;
		x2 = (PLAYERY * 2 + 1)*(players[i].health*0.01);
		if(players[i].state == 0 || players[i].state == 3 || players[i].state == 4)PrintRectangleScale(x1, y1, x1 + x2, y1 + 1, MixColor(0xFF0000, 0xFF00, players[i].health*0.01));
		y1 = players[i].y + PLAYERY + 15;
		x1 = players[i].x;
		y2 = 20;
		Transform(x1, y1);
		y2 *= GetScale();
		PutStringMidH(players[i].name, x1, y1, y2, 0);
	}
	PrintResources();
	PrintParticals();
	PrintBullets(id);
	PrintBases(players[id].side);
	PrintPlayerStateblank(id);
	PrintMES(players[id].side);
}
void TrackParticalUpdate(Partical& part, double ms)
{
	for (int i = 0; i < ms; i++)
	{
		part.x = part.x*TRACKCOE + players[part.color1].x*(1.0-TRACKCOE);
		part.y = part.y*TRACKCOE + players[part.color1].y*(1.0-TRACKCOE);
		if (Vector(part.x - players[part.color1].x, part.y - players[part.color1].y).Dist() <= PLAYERX)part.life = part.lim + 1.0;
		else part.life = 0;
	}
}
void HaloParticalPrint(Partical& part)
{
	PrintHaloScale(part.x, part.y, 2.0, part.color2);
}
void ExplosionUpdate(Bullet&bullet, int id, int num)
{
	double radius = bullet.speedy*(bullet.life / bullet.lim);
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].side == players[bullet.from].side&&i != bullet.from)continue;
		if (players[i].state == -1)continue;
		double x1, y1, x2, y2, x, y;
		x1 = players[i].x - PLAYERX;
		x2 = players[i].x + PLAYERX;
		y1 = players[i].y - PLAYERY;
		y2 = players[i].y + PLAYERY;
		x = bullet.x;
		y = bullet.y;
		if (MinDist(x, y, x1, y1, x2, y2) <= radius)
		{
			if (i == bullet.from)
			{
				AddHealth(i, -bullet.speedx*0.3, bullet.from);
			}
			else AddHealth(i, -bullet.speedx, bullet.from);
			if (players[i].health == 0.0)
			{
				KillPlayer(i, bullet.from);
			}
		}
	}
	bullet.life += 0.001;
}
int GetBulletColor(Bullet&bullet, int id)
{
	if (bullet.from == id)return 0xFFFF00;
	else if (players[bullet.from].side == players[id].side)return 0x8888FF;
	else return 0xFF8888;
}
void ExplosionPrint(Bullet&bullet, int id)
{
	PrintHaloScale(bullet.x, bullet.y, bullet.speedy*(bullet.life / bullet.lim), GetBulletColor(bullet, id));
}
void RifleUpdate(Bullet&bullet, int id, int num)
{
	for (int t = 0; t < 10; t++)
	{
		//bullet.speedy -= 0.2;
		bullet.x += bullet.speedx*0.0001;
		bullet.y += bullet.speedy*0.0001;
		AddPartical(bullet.x, bullet.y, 2.0, 0.0, 0.01, GetBulletColor(bullet, id), 0xFF0000, MarkParticalPrint, MarkParticalUpdate);
		for (int i = 0; i < cntp; i++)
		{
			if (players[i].side == players[bullet.from].side)continue;
			if (players[i].state == -1)continue;
			double x1, y1, x2, y2;
			x1 = players[i].x - PLAYERX;
			x2 = players[i].x + PLAYERX;
			y1 = players[i].y - PLAYERY;
			y2 = players[i].y + PLAYERY;
			if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= 1.0)
			{
				bullet.life = bullet.lim + 1;
				AddHealth(i, -9.5, bullet.from);
				if (players[i].health == 0.0)KillPlayer(i, bullet.from);
				return;
			}
		}
		for (int i = 0; i < cntb; i++)
		{
			double x1, y1, x2, y2;
			x1 = block[i][0][0];
			x2 = block[i][0][1];
			y1 = block[i][1][0];
			y2 = block[i][1][1];
			if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= 1.0)
			{
				bullet.life = bullet.lim + 1;
				if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= 1.0)
				{
					bullet.life = bullet.lim + 1;
					for (int j = 0; j < 10; j++)
					{
						double d = atan2(bullet.speedx, bullet.speedy), r = RandDouble(1.0, 300.0);
						d = RandDouble(d - PI*0.5, d + PI*0.5);
						AddPartical(bullet.x, bullet.y, sin(d)*r, cos(d)*r, 0.1, 0x000000, 1, DirtParticalPrint, DirtParticalUpdate);
					}
					return;
				}
				return;
			}
		}
	}
}
void RiflePrint(Bullet&bullet, int id)
{
	;
}

void SniperUpdate(Bullet&bullet, int id, int num)
{
	for (int t = 0; t < 40; t++)
	{
		bullet.x += bullet.speedx*0.000025;
		bullet.y += bullet.speedy*0.000025;
		AddPartical(bullet.x, bullet.y, 2.0, 0.0, 0.02, (players[bullet.from].side == players[id].side) ? 0xAAAAAA : 0xAA0000, 0xFFFFFF, MarkParticalPrint, MarkParticalUpdate);
		//AddPartical(bullet.x, bullet.y, 2.0, 40.0, 0.2, 0xAA0000, 0xFFFFFF, SmokeParticalPrint, SmokeParticalUpdate);
		for (int i = 0; i < cntp; i++)
		{
			if (players[i].side == players[bullet.from].side)continue;
			if (players[i].state == -1)continue;
			double x1, y1, x2, y2;
			x1 = players[i].x - PLAYERX;
			x2 = players[i].x + PLAYERX;
			y1 = players[i].y - PLAYERY;
			y2 = players[i].y + PLAYERY;
			if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= 1.0)
			{
				bullet.life = bullet.lim + 1;
				AddHealth(i, -80, bullet.from);
				if (players[i].health == 0.0)KillPlayer(i, bullet.from);
				return;
			}
		}
		for (int i = 0; i < cntb; i++)
		{
			double x1, y1, x2, y2;
			x1 = block[i][0][0];
			x2 = block[i][0][1];
			y1 = block[i][1][0];
			y2 = block[i][1][1];
			if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= 1.0)
			{
				bullet.life = bullet.lim + 1;
				for (int j = 0; j < 20; j++)
				{
					double d = RandDouble(j*(0.1*PI), (j+1)*(0.1*PI)), r = RandDouble(1.0, 300.0);
					
					AddPartical(bullet.x, bullet.y, sin(d)*r, cos(d)*r, 1.0, 0x000000, rand() % 4 + 1, DirtParticalPrint, DirtParticalUpdate);
				}
				return;
			}
		}
	}
}
void SniperPrint(Bullet&bullet, int id)
{
	;
}

void MeteoriteUpdate(Bullet&bullet, int id, int num)
{
	bullet.y += bullet.speedy*0.001;
	for(int i=0;i<5;i++)
	{
		double d = RandDouble(0.0, 2*PI), mx, my;
		mx = bullet.speedx*sin(d);
		my = bullet.speedx*cos(d);

		AddPartical(bullet.x + mx, bullet.y + my, 10.0, 40.0, (1.0 - abs(mx / bullet.speedx))*0.4, 0xFF0000, 0, SmokeParticalPrint, SmokeParticalUpdate);
		//AddPartical(bullet.x + mx, bullet.y + my, 0.0, 0.0, 1.0, 0xFF0000, 0xFFFF00, FireParticalPrint, FireParticalUpdate);
	}
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].side == players[bullet.from].side)continue;
		if (players[i].state == -1)continue;
		double x1, y1, x2, y2;
		x1 = players[i].x - PLAYERX;
		x2 = players[i].x + PLAYERX;
		y1 = players[i].y - PLAYERY;
		y2 = players[i].y + PLAYERY;
		if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= bullet.speedx)
		{
			AddHealth(i, -1000.0, bullet.from);
			if(players[i].health<=0.0)KillPlayer(i, bullet.from);
		}
	}
}
void MeteoritePrint(Bullet&bullet, int id)
{
	//PrintRectangleScale(bullet.x - bullet.speedx*1.1, border[1][0], bullet.x + bullet.speedx*1.1, border[1][1], GetBulletColor(bullet, id), 0.3);
	PrintCircleScale(bullet.x, bullet.y, bullet.speedx*0.9, GetBulletColor(bullet, id));
}

void RocketUpdate(Bullet&bullet, int id, int num)
{
	bullet.speedx *= 1.002;
	bullet.speedy *= 1.002;
	double d = sqrt(bullet.speedx*bullet.speedx + bullet.speedy*bullet.speedy);
	if (d > 2000.0)
	{
		d = 2000.0 / d;
		bullet.speedx *= d;
		bullet.speedy *= d;
	}
	bullet.x += bullet.speedx*0.001;
	bullet.y += bullet.speedy*0.001;

	AddPartical(bullet.x, bullet.y, 5.0, 140.0, 0.15, 0, 0, SmokeParticalPrint, SmokeParticalUpdate);
	AddPartical(bullet.x, bullet.y, 5.0, 40.0, 0.05, 0xFF0000, 0, SmokeParticalPrint, SmokeParticalUpdate);
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].side == players[bullet.from].side)continue;
		if (players[i].state == -1)continue;
		double x1, y1, x2, y2;
		x1 = players[i].x - PLAYERX;
		x2 = players[i].x + PLAYERX;
		y1 = players[i].y - PLAYERY;
		y2 = players[i].y + PLAYERY;
		if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= bullet.lim*0.5)
		{
			bullet.life = bullet.lim + 1;
			AddBullet(bullet.from, bullet.x, bullet.y, 0.4, 70.0, 0.15, 1);
			return;
		}
	}
	for (int i = 0; i < cntb; i++)
	{
		double x1, y1, x2, y2;
		x1 = block[i][0][0];
		x2 = block[i][0][1];
		y1 = block[i][1][0];
		y2 = block[i][1][1];
		if (MinDist(bullet.x, bullet.y, x1, y1, x2, y2) <= bullet.lim*0.5)
		{
			bullet.life = bullet.lim + 1;
			AddBullet(bullet.from, bullet.x, bullet.y, 0.4, 70.0, 0.15, 1);
			return;
		}
	}
	for (int i = num + 1; i < cnt_bullet; i++)
	{
		if (players[bullets[i].from].side == players[bullet.from].side || bullets[i].type != 0)continue;
		if (Vector(bullets[i].x - bullet.x, bullets[i].y - bullet.y).Dist() <= (bullet.lim + bullets[i].lim)*0.5)
		{
			bullet.life = bullet.lim + 1;
			AddBullet(bullet.from, bullet.x, bullet.y, 0.4, 70.0, 0.15, 1);
			bullets[i].life = bullets[i].lim + 1;
			AddBullet(bullets[i].from, bullets[i].x, bullets[i].y, 0.4, 70.0, 0.15, 1);
			return;
		}
	}
}
void RocketPrint(Bullet&bullet, int id)
{
	int color = GetBulletColor(bullet, id);
	PrintHaloScale(bullet.x, bullet.y, bullet.lim, color);
	PrintCircleScale(bullet.x, bullet.y, bullet.lim, color, 0.3);
}
void GravityBallUpdate(Bullet&bullet, int id, int num)
{
	bullet.speedx *= 0.995;
	bullet.speedy *= 0.995;
	bullet.x += bullet.speedx*0.001;
	bullet.y += bullet.speedy*0.001;
	Vector v(bullet.speedx, bullet.speedy), v1; double d = frame*DEGREE(1.0);
	int color = GetBulletColor(bullet, id);
	v1.x = sin(d)*50.0; v1.y = cos(d)*50.0;
	AddPartical(bullet.x + v1.x, bullet.y + v1.y, 2.0, 0.0, 0.1, color, 0xFFFF00, MarkParticalPrint, MarkParticalUpdate);
	d += PI*2.0 / 3.0;
	v1.x = sin(d)*50.0; v1.y = cos(d)*50.0;
	AddPartical(bullet.x + v1.x, bullet.y + v1.y, 2.0, 0.0, 0.1, color, 0xFFFF00, MarkParticalPrint, MarkParticalUpdate);
	d += PI*2.0 / 3.0;
	v1.x = sin(d)*50.0; v1.y = cos(d)*50.0;
	AddPartical(bullet.x + v1.x, bullet.y + v1.y, 2.0, 0.0, 0.1, color, 0xFFFF00, MarkParticalPrint, MarkParticalUpdate);
	for (int i = 0; i < cnt_bullet; i++)
	{
		if (!bullets[i].type)
		{
			Vector v = Vector(bullet.x, bullet.y) - Vector(bullets[i].x, bullets[i].y);
			double d = v.Dist();
			if (d < 200.0)
			{
				v = v.Zoom(40.0);
				bullets[i].speedx += v.x;
				bullets[i].speedy += v.y;
			}
		}
		else if (bullets[i].type == 4 && players[bullets[i].from].side != players[bullet.from].side)
		{
			Vector v = Vector(bullet.x, bullet.y) - Vector(bullets[i].x, bullets[i].y);
			double d = v.Dist();
			if (d < 200.0)
			{
				bullets[i].life = bullets[i].lim + 1;
			}
		}
	}
	bullet.life += 0.001;
	if (bullet.life > bullet.lim)
	{
		for (int i = 0; i < cnt_bullet; i++)
		{
			if (!bullets[i].type)
			{
				Vector v = Vector(bullet.x, bullet.y) - Vector(bullets[i].x, bullets[i].y);
				double d = v.Dist();
				if (d < 200.0)
				{
					v = Vector(players[bullet.from].ctrl.mousepos.x, players[bullet.from].ctrl.mousepos.y) - Vector(bullets[i].x, bullets[i].y);
					v = v.Zoom(1500.0);
					bullets[i].speedx = v.x;
					bullets[i].speedy = v.y;
				}
			}
		}
	}
}
void GravityBallPrint(Bullet&bullet, int id)
{
	int color = GetBulletColor(bullet, id);
	PrintCircleScale(bullet.x, bullet.y, 30.0, color, 0.2);
	PrintHaloScale(bullet.x, bullet.y, 30.0, color);
}

void ManaBurnUpdate(Bullet&bullet, int id, int num)
{
	double n = 5.0, alpha = bullet.life / bullet.lim; int color;
	if (players[bullet.from].side == players[id].side)color = 0x880088;
	else color = 0xFF0000;
		alpha = sin(alpha*PI);
	for (int i = 0; i < n; i++)
	{
		double d = 2 * PI*(i / n) + DEGREE(bullet.life / bullet.lim*480.0), nx = sin(d)*200.0*alpha, ny = cos(d)*200.0*alpha;
		AddPartical(bullet.x + nx, bullet.y + ny, 5.0, 0.0, 0.5, color, 0xFF00FF, MarkParticalPrint, MarkParticalUpdate);
	}
	bullet.life += 0.001;
	if (players[bullet.from].state != -1)
	{
		bullet.x = players[bullet.from].x;
		bullet.y = players[bullet.from].y;
		for (int i = 0; i < cntp; i++)
		{
			if (players[i].state != -1 && players[i].state != 1 && players[i].side != players[bullet.from].side)
			{
				if (Vector(players[bullet.from].x - players[i].x, players[bullet.from].y - players[i].y).Dist() <= 200.0*alpha)
				{
					double en = players[i].energy;
					AddEnergy(i, -en);
					AddHealth(i, -en*0.05, bullet.from);
					if (players[i].health == 0.0)KillPlayer(i, bullet.from);
					AddEnergy(bullet.from, en);
					AddHealth(bullet.from, en*0.05, bullet.from);
				}
			}
		}
	}
	else bullet.life = bullet.lim + 1.0;
}

void ManaBurnPrint(Bullet&bullet, int id)
{
	double n = 16.0, alpha = bullet.life / bullet.lim;
	if (alpha < 0.1)alpha = alpha*10.0;
	else alpha = (1.0 - alpha) / 0.9;
	PrintHaloScale(bullet.x, bullet.y, 200.0*alpha, 0xFFFF00);
}

void Normal_Rocket(int id)
{
	if (players[id].cd[0] == 0.0)
	{
		Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(2000.0);
		//v *= 3.0;
		players[id].cd[0] = 1.0;
		AddBullet(id, players[id].x, players[id].y, v.x, v.y, 10.0, 0);
		return;
	}
}

void Normal_Rifle(int id)
{
	if (players[id].cd[6] == 0.0)
	{
		Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(3000.0);
		if(players[id].cd[3]==0.0)players[id].cd[6] = 0.11;
		else players[id].cd[6] = 0.01;
		v.Rotate(RandDouble(DEGREE(-2.0), DEGREE(2.0)));
		AddBullet(id, players[id].x, players[id].y, v.x, v.y, 10.0, 4);
		return;
	}
}


void Normal_Sniper(int id)
{
	if (players[id].cd[6] == 0.0)
	{
		Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(30000.0);
		players[id].cd[6] = 2.0;
		//v.Rotate(RandDouble(DEGREE(-2.0), DEGREE(2.0)));
		AddBullet(id, players[id].x, players[id].y, v.x, v.y, 10.0, 6);
		return;
	}
}

void Skill_GravityBall(int id)
{
	if (players[id].cd[1] == 0.0)
	{
		Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(600.0);
		AddBullet(id, players[id].x, players[id].y, v.x, v.y, 6.5, 2);
		players[id].cd[1] = 20.0;
		return;
	}
}

void Skill_Meteorite(int id)
{
	if (players[id].cd[1] == 0.0)
	{
		AddBullet(id, players[id].ctrl.mousepos.x, border[1][1]-1, 50.0, -700.0, 10.0, 5);
		players[id].cd[1] = 20.0;
		return;
	}
}

void Skill_Sting(int id)
{
	if (players[id].cd[2] == 0.0)
	{
		players[id].state = 2;
		players[id].cont = 0.2;
		players[id].cd[2] = 8.0;
		Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(1500);
		players[id].speedx = v.x;
		players[id].speedy = v.y;
	}
}

void Skill_Invisible(int id)
{
	if (players[id].cd[2] == 0.0)
	{
		players[id].state = 3;
		players[id].cont = 5;
		players[id].cd[2] = 8.0;
		return;
	}
}

void Ultimate_Manaburn(int id)
{
	if (players[id].energy >= 10000.0&&players[id].cd[3] == 0.0)
	{
		players[id].cd[3] = 1.0;
		AddEnergy(id, -10000.0);
		AddBullet(id, players[id].x, players[id].y, 0.0, 0.0, 0.5, 3);
	}
}

void Ultimate_Barrage(int id)
{
	if (players[id].energy >= 10000.0&&players[id].cd[3] == 0.0)
	{
		players[id].state = 4;
		players[id].cont = 2.0;
		players[id].cd[3] = 1.0;
		AddEnergy(id, -10000.0);
		return;
	}
}

void Ultimate_Crazyshoot(int id)
{
	if (players[id].energy >= 10000.0&&players[id].cd[3] == 0.0)
	{
		players[id].cd[3] = 6.0;
		AddEnergy(id, -10000.0);
	}
}

void Contskill_Sacrifice(int id)
{
	for (int i = 0; i < cntp; i++)
	{
		if (players[i].side == players[id].side || players[i].state == -1)continue;
		if (Vector(players[i].x - players[id].x, players[i].y - players[id].y).Dist() < 300.0)
		{
			AddHealth(i, -0.01, id);
			AddPartical(players[i].x + RandDouble(-PLAYERX, PLAYERX), players[i].y + RandDouble(-PLAYERY, PLAYERY), 0.0, 0.0, 1.0, 0xFF0000, 0xFFFF00, FireParticalPrint, FireParticalUpdate);
			if (players[i].health == 0.0)KillPlayer(i, id);
		}
	}
}

void Contskill_Jetpack(int id)
{
	if (players[id].speedy > 200.0)return;
	players[id].speedy += 2.5;
	if (players[id].speedy < 0.0)players[id].speedy += 1.5;
}

void UseSkill(int id)
{
	if (players[id].ctrl.press[VK_LBUTTON])
	{
		//Normal_Rocket(id);
		//Normal_Rifle(id);
		NormalSkill[players[id].type](id);
	}
	
	if (players[id].ctrl.press[VK_SHIFT])
	{
		RareSkill[players[id].type](id);
	}
	if (players[id].ctrl.press['Q'])
	{
		LegendSkill[players[id].type](id);
	}
	if (players[id].ctrl.press[VK_RBUTTON] || players[id].ctrl.press['E'])
	{
		EpicSkill[players[id].type](id);
	}
	/*
	if (players[id].ctrl.press[VK_RBUTTON])
	{
		//Skill_GravityBall(id);
		Skill_Meteorite(id);
	}
	if (players[id].ctrl.press['S'])
	{
		Skill_Invisible(id);
	}*/
}
void UseContinuedSkill(int id)
{
	if (players[id].ctrl.press['W'] || players[id].type == 1)
	{
		ContSkill[players[id].type](id);
	}
	/*if (players[id].ctrl.press['W'])
	{
		Contskill_Sacrifice(id);
	}
	if (players[id].ctrl.press[VK_SPACE])
	{
		Contskill_Jetpack(id);
	}//*/
}
void UpdatePlayer(int id, int watchid)
{
	if (players[id].state == 0 || players[id].state == 1 || players[id].state == 3 || players[id].state == 4)
		UseContinuedSkill(id);
	if (players[id].cont == 0.0)
	{
		if (players[id].state == 0)UseSkill(id);
		else if (players[id].state == -1)
		{
			players[id].state = 1;
			players[id].health = 100.0;
			players[id].cont = 2.0;
			//players[id].energy = 2000.0;
			players[id].x = rebornpos[players[id].side][0];
			players[id].y = rebornpos[players[id].side][1];

			players[id].type = (rand() ^ rand()) % 2;
			players[id].speedx = 0.0;
			players[id].speedy = 0.0;
		}
		else if (players[id].state == 1)
		{
			players[id].state = 0;
		}
		else if (players[id].state == 2)
		{
			players[id].cont = 0.2;
			players[id].state = 0;
			players[id].speedx = 0.0;
			players[id].speedy = 0.0;
			return;
		}
		else if (players[id].state == 3)
		{
			players[id].state = 0;
		}
		else if (players[id].state == 4)
		{
			players[id].state = 0;
			players[id].speedx = 0.0;
			players[id].speedy = 0.0;
		}
	}
	if (players[id].state == 2)
	{
		/*players[id].x += players[id].speedx;
		players[id].y += players[id].speedy;//*/
		/*Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
		v = v.Zoom(1500);
		players[id].speedx = v.x;
		players[id].speedy = v.y;//*/
		int color;
		if (watchid == id)color = 0xFF00;
		else if (players[id].side == players[watchid].side)color = 0xFF;
		else color = 0xFF0000;
		AddPartical(players[id].x, players[id].y, 10.0, 0.0, 0.2, color, 0xFFFF00, MarkParticalPrint, MarkParticalUpdate);
		for (int i = 0; i < cntp; i++)
		{
			if (players[id].side != players[i].side&&players[i].state != -1)
			{
				double x1, y1, x2, y2;
				x1 = players[i].x - PLAYERX;
				x2 = players[i].x + PLAYERX;
				y1 = players[i].y - PLAYERY;
				y2 = players[i].y + PLAYERY;
				if (MinDist(players[id].x, players[id].y, x1, y1, x2, y2) <= 20.0)
				{
					AddHealth(i, -3.0, id);
					if (players[i].health == 0.0)
						KillPlayer(i, id);
				}
			}
		}

	}
	if (players[id].state == 4)
	{
		if (frame % 100 == 0)
		{
			Vector v(players[id].ctrl.mousepos.x - players[id].x, players[id].ctrl.mousepos.y - players[id].y);
			v = v.Zoom(2000.0);
			v.Rotate(RandDouble(DEGREE(-10.0), DEGREE(10.0)));
			AddBullet(id, players[id].x, players[id].y, v.x, v.y, 10.0, 0);
		}
	}
}
void UpdateWorld(int ms, int id)
{
	for (int i = 0; i < ms; i++)
	{
		frame++;
		if (frame % 5 == 0)
		{
			AddPartical(RandDouble(rebornpos[0][0] - PLAYERX, rebornpos[0][0] + PLAYERX), RandDouble(rebornpos[0][1] - PLAYERY, rebornpos[0][1] + PLAYERY), 0.7, 0.0, RandDouble(0.5, 1.0), 0xFFFF00, 0xFF0000, LightspotParticalPrint, LightspotParticalUpdate);
			AddPartical(RandDouble(rebornpos[1][0] - PLAYERX, rebornpos[1][0] + PLAYERX), RandDouble(rebornpos[1][1] - PLAYERY, rebornpos[1][1] + PLAYERY), 0.7, 0.0, RandDouble(0.5, 1.0), 0xFFFF00, 0xFF0000, LightspotParticalPrint, LightspotParticalUpdate);
		}
		/*if (frame % 500 == 0)
		{
			AddBullet(0, players[0].x, players[0].y, -400, 0, 10.0, 0);
		}//*/
		for (int i = 0; i < cntp; i++)
		{
			if (players[i].state != -1/* && players[i].state != 4*/)
			{
				if (players[i].state != 2)
				{
					players[i].speedy -= 2;
					players[i].speedx = 0.0;
					if (players[i].state == 3)
					{
						if (players[i].ctrl.press['A'])players[i].speedx += -800;
						if (players[i].ctrl.press['D'])players[i].speedx += 800;
					}
					else
					{
						if (players[i].ctrl.press['A'])players[i].speedx += -400;
						if (players[i].ctrl.press['D'])players[i].speedx += 400;
					}
				}
				double movex = players[i].speedx*0.001, movey = players[i].speedy*0.001;
				double l = players[i].x - PLAYERX - 1, u = players[i].y + PLAYERY + 1, r = players[i].x + PLAYERX + 1, d = players[i].y - PLAYERY - 1;
				for (int j = 0; j < cntb; j++)
				{
					if (l<block[j][0][1] && r>block[j][0][0])
					{
						if (movey < 0)
						{
							//movey = 0.0;
							if (block[j][1][1] - d <= 0.0)movey = max(movey, block[j][1][1] - d);
						}
						else if (movey > 0)
						{
							if (block[j][1][0] - u >= 0.0)movey = min(movey, block[j][1][0] - u);
						}
					}
				}
				players[i].y += movey;
				l = players[i].x - PLAYERX - 1, u = players[i].y + PLAYERY + 1, r = players[i].x + PLAYERX + 1, d = players[i].y - PLAYERY - 1;
				for (int j = 0; j < cntb; j++)
				{
					if (d<block[j][1][1] && u>block[j][1][0])
					{
						if (movex < 0)
						{
							//movey = 0.0;
							if (block[j][0][1] - l <= 0.0)movex = max(movex, block[j][0][1] - l);
						}
						else if (movex > 0)
						{
							if (block[j][0][0] - r >= 0.0)movex = min(movex, block[j][0][0] - r);
						}
					}
				}
				players[i].x += movex;
				if (players[i].state != 2)
				{
					players[i].speedx = movex * 1000.0;
					if (players[i].speedy < 0.0)
						if (players[i].ctrl.press[VK_SPACE] && movey == 0.0)
						{
							if (players[i].state == 3)players[i].speedy = 600.0*sqrt(2);
							else players[i].speedy = 600.0;
						}
						else players[i].speedy = movey * 1000.0;
					else players[i].speedy = movey * 1000.0;
				}
			}
			if (players[i].name == string("20162503"))
				AddEnergy(i, 1.1);
			AddEnergy(i, 0.1);
			

			if (!InBorder(players[i].x, players[i].y) && players[i].state != -1)KillPlayer(i, i);

			for (int j = 0; j < cntr; j++)
			{
				if (Vector(resources[j][0] - players[i].x, resources[j][1] - players[i].y).Dist() <= 150.0)
				{
					if (players[i].state != -1)if (frame % 5 == 0)AddPartical(RandDouble(players[i].x - PLAYERX, players[i].x + PLAYERX), RandDouble(players[i].y - PLAYERY, players[i].y + PLAYERY), 0.5, 0.0, 1.0, 0x00AAFF, 0, LightspotParticalPrint, LightspotParticalUpdate);
					AddHealth(i, 0.02, i);
				}
			}
			players[i].cont -= 0.001;
			players[i].cont = max(players[i].cont, 0.0);
			for (int j = 0; j < 10; j++)
			{
				players[i].cd[j] -= 0.001;
				players[i].cd[j] = max(players[i].cd[j], 0.0);
			}
			UpdatePlayer(i, id);
		}
		UpdateBullets(id);
		UpdateBases();
		UpdateResources();
		UpdateParticals();
		UpdMES();
	}
}

void PackWorldInfo(SendBag & sendbag)
{
	ResetSendbag(&sendbag);
	AddInfo(&sendbag, "WRLDINFO", 8);
	AddInt(&sendbag, frame);
	AddDouble(&sendbag, damageprop[0]);
	AddDouble(&sendbag, damageprop[1]);
	AddInt(&sendbag, cntp);
	for (int i = 0; i < cntp; i++)
		AddInfo(&sendbag, (char*)&players[i], sizeof(Player));
	AddInt(&sendbag, cnt_bullet);
	for (int i = 0; i < cnt_bullet; i++)
		AddInfo(&sendbag, (char*)&bullets[i], sizeof(Bullet));
	AddInt(&sendbag, cnt_MES);
	for (int i = 0; i < cnt_MES; i++)
		AddInt(&sendbag, MES_from[i]),
		AddInt(&sendbag, MES_to[i]),
		AddInt(&sendbag, MES_time[i]);
}

void UnpackWorldInfo(Message & message, int id)
{
	char *p = message.buf;
	p += 8;
	int new_frame = GetInt(p);
	UpdateWorld(new_frame-frame,id);
	damageprop[0] = GetDouble(p);
	damageprop[1] = GetDouble(p);
	cntp = GetInt(p);
	for (int i = 0; i < cntp; i++)GetInfo(p, sizeof(Player), (char*)&players[i]);
	cnt_bullet = GetInt(p);
	for (int i = 0; i < cnt_bullet; i++)GetInfo(p, sizeof(Bullet), (char*)&bullets[i]);
	cnt_MES = GetInt(p);
	for (int i = 0; i < cnt_MES; i++)
		MES_from[i] = GetInt(p),
		MES_to[i] = GetInt(p),
		MES_time[i] = GetInt(p);
	//UpdateWorld();
}

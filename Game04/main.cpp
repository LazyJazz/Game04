#include"Draw.h"
#include<iostream>
#include<cstdio>
#include"SocketLib.h"
#include"World.h"
#include"Control.h"
#include"API.h"
#define ScrX getwidth()
#define ScrY getheight()
using namespace std;
char str[128];
void CreateWorld()
{
	SetBorder(-1500.0, -500.0, 1500.0, 2000.0);
	AddBlock(-1110, 500, -900, 490);
	AddBlock(-850, 420, -750, 410);
	AddBlock(-700, 400, -600, 390);
	AddBlock(-550, 470, -450, 460);
	AddBlock(-400, 550, -300, 540);
	AddBlock(-250, 490, -150, 480);
	AddBlock(-100, 410, 100, 400);
	AddBlock(-800, 580, -700, 570);
	AddBlock(-650, 600, -550, 590);
	AddBlock(-600, 320, -500, 310);
	AddBlock(-450, 310, -350, 300);
	AddBlock(-300, 330, -200, 320);
	AddBlock(-500, 630, -400, 620);
	AddBlock(-250, 640, -150, 630);
	AddBlock(-100, 700, 100, 690);
	AddBlock(-100, 780, -20, 770);
	AddBlock(-100, 860, -110, 770);
	AddBlock(-110, 940, -90, 930);
	AddBlock(-90, 960, -80, 930);
	AddBlock(-90, 960, -60, 950);
	AddBlock(-70, 980, -60, 950);
	AddBlock(-70, 980, -40, 970);
	AddBlock(-50, 1000, -40, 970);
	AddBlock(-50, 1000, -20, 990);
	AddBlock(-30, 1020, -20, 990);
	AddBlock(-30, 1020, 0, 1010);
	AddBlock(-30, 860, 0, 850);
	AddBlock(-1000, 860, -200, 850);
	AddBlock(-80, 280, 0, -40);
	AddBlock(-80, 200, -100, 190);
	AddBlock(-240, 120, 00, -40);
	AddBlock(-1040, 280, -160, 200);
	AddBlock(-400, 120, -320, -120);
	AddBlock(-240, -120, 0, -200);
	AddBlock(-320, -40, -300, -50);
	AddBlock(-260, 40, -240, 30);
	AddBlock(-400, 40, -480, -40);
	AddBlock(-560, 200, -640, 40);
	AddBlock(-560, 200, -480, 120);
	AddBlock(-560, -40, -640, -120);
	AddBlock(-560, -115, -540, -120);
	AddBlock(-720, -120, -1200, -200);
	AddBlock(-1200, -200, 0, -280);
	AddBlock(-1040, 120, -720, -40);
	AddBlock(-720, 40, -700, 30);
	AddBlock(-1120, -120, -1200, 400);
	AddBlock(-1280, -280, -1200, 480);
	AddBlock(-1120, -40, -1100, -45);
	AddBlock(-1040, 40, -1060, 35);
	AddBlock(-1120, 120, -1100, 115);
	AddBlock(-1120, 200, -1100, 195);
	AddBlock(-1120, 280, -1100, 275);
	AddBlock(-1120, 360, -1100, 355);
	AddResource(-700, 700);
	AddResource(700, 700);
	AddResource(-1200, 600);
	AddResource(1200, 600);
	AddResource(0.0, 500);
	SetRebornPoint(-1000.0, 530, 1000.0, 530);
	SetBase(-1000.0, 600, 1000.0, 600);
}
SockServer srv;
CLIENT cln;
string name[64];
int lastupdate[64];
int side[64];
long long key[64];
CSNAPSHOT ctrl[64];
int cntu = 0;
int state = 0;
Message message;
SendBag datareply, sendbag;
long long mykey;
int CheckKey(long long nkey)
{
	for (int i = 0; i < cntu; i++)
		if (nkey == key[i])return i;
	return -1;
}
long long Regist(string usrname)
{
	if (cntu == 64)return -1;
	long long nkey = 0;
	for (long long i = 0; i < 64; i++)nkey |= (long long)(rand() & 1) << i;
	while (nkey == -1 || CheckKey(nkey) != -1)
	{
		for (long long i = 0; i < 64; i++)nkey |= (long long)(rand() & 1) << i;
	}
	for (int i = 0; i < cntu; i++)
	{
		if (name[i]==usrname)
		{
			return -1;
		}
	}
	name[cntu] = usrname;
	lastupdate[cntu] = clock();
	side[cntu] = 0;
	key[cntu] = nkey;
	cntu++;
	return nkey;
}
void SetInput(long long nkey,CSNAPSHOT ct)
{
	int id = CheckKey(nkey);
	if (id != -1)
	{
		ctrl[id] = ct;
		lastupdate[id] = clock();
	}
}
void DeleteUser(long long nkey)
{
	int ncntu = 0;
	for (int i = 0; i < cntu; i++)
	{
		if (key[i] != nkey)
		{
			name[ncntu] = name[i];
			lastupdate[ncntu] = lastupdate[i];
			side[ncntu] = side[i];
			key[ncntu] = key[i];
			ctrl[ncntu] = ctrl[i];
			ncntu++;
		}
	}
	cntu = ncntu;
}
void KickUser()
{
	int ncntu = 0;
	for (int i = 0; i < cntu; i++)
	{
		if (clock() - lastupdate[i] <= 3000)
		{
			name[ncntu] = name[i];
			lastupdate[ncntu] = lastupdate[i];
			side[ncntu] = side[i];
			key[ncntu] = key[i];
			ctrl[ncntu] = ctrl[i];
			ncntu++;
		}
	}
	cntu = ncntu;
}
void ChangeSide(long long nkey)
{
	for (int i = 0; i < cntu; i++)if (key[i] == nkey)side[i] = side[i] ^ 1;
}
void PackUserInfo(SendBag &sendbag)
{
	ResetSendbag(&sendbag);
	AddInfo(&sendbag, "USERINFO", 8);
	AddInt(&sendbag, cntu);
	for (int i = 0; i < cntu; i++)
	{
		AddString(&sendbag, name[i].c_str(), name[i].length());
		AddInt(&sendbag, side[i]);
		AddLongLong(&sendbag, key[i]);
	}
}
void UnpackUserInfo(Message &message)
{
	char *p = message.buf;
	p += 8;
	cntu = GetInt(p);
	char str[128];
	for (int i = 0; i < cntu; i++)
	{
		memset(str, 0, sizeof(str));
		GetString(p, str);
		name[i] = str;
		side[i] = GetInt(p);
		key[i] = GetLongLong(p);
	}
}
void ProcessMessage(Message &message,char *usrname)
{
	char head[9], *p=message.buf;
	head[8] = 0;
	GetInfo(p, 8, head);
	if (string(head) == "USERINFO")UnpackUserInfo(message), state = 0;
	if (string(head) == "REGISTER")
	{
		char str[128]; memset(str, 0, sizeof(str));
		GetString(p, str);
		long long ret = Regist(str);
		if (state == 1)ret = -1;
		ResetSendbag(&sendbag);
		AddInfo(&sendbag, "KEYVALUE", 8);
		AddLongLong(&sendbag, ret);
		SendMessage(message.clnsock, sendbag.buf, sendbag.len);
	}
	if (string(head) == "KEYVALUE")
	{
		mykey = GetLongLong(p);
	}
	if (string(head) == "SETCTROL")
	{
		CSNAPSHOT snapshot; long long nkey;
		nkey = GetLongLong(p);
		GetInfo(p, sizeof(CSNAPSHOT), (char*)&snapshot);
		SetInput(nkey, snapshot);
	}
	if (string(head) == "CHANGESD")
	{
		long long nkey = GetLongLong(p);
		ChangeSide(nkey);
	}
	if (string(head) == "WRLDINFO")UnpackWorldInfo(message, CheckPlayerId(usrname)), state = 1;
}
void Game04Server(char *usrname)
{
	InitSocket();
	StartServer(&srv, 62503);//*/
	Button Exit, Start, ChangeS;
	Exit.SetButton("Exit", 0.02, 0.9, 0.3, 0.08);
	Start.SetButton("Start", 0.68, 0.9, 0.3, 0.08);
	ChangeS.SetButton("ChangeSide", 0.02, 0.02, 0.3, 0.06);
	cntu = 0;
	mykey = Regist(usrname);
	int cntside[2];
	CSNAPSHOT snapshot;
	int lastupdate;
	double x1, y1, x2, y2;// dd = 0.0;
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		cleardevice();
		UpdateServer(&srv);
		while (!srv.MessageQueueEmpty())
		{
			message = srv.TakeMessage();
			ProcessMessage(message, usrname);
		}//*/
		TakeSnapshot(snapshot);
		SetInput(mykey, snapshot);
		if (state == 0)
		{
			cntside[0] = 0;
			cntside[1] = 0;
			double x2, y2;
			x2 = getwidth()*0.4;
			y2 = getheight()*0.04;
			for (int i = 0; i < cntu; i++)
			{
				double x1, y1;
				x1 = 0.05;
				if (side[i])x1 += 0.5;
				x1 *= getwidth();
				y1 = 0.1;
				y1 += 0.05*cntside[side[i]]++;
				y1 *= getheight();
				PrintRectangle(x1, y1, x1 + x2, y1 + y2, side[i] ? 0xFF0000 : 0xFF, 0.3);
				PutStringH(name[i].c_str(), x1, y1, y2, key[i] == mykey ? 0xFF : 0);
			}
			if (Exit.Print())
			{
				StopServer(&srv);
				return;
			}
			if (Start.Print())
			{
				state = 1;
				//AddPlayer(1, "Robot");
				for (int i = 0; i < cntu; i++)
				{
					AddPlayer(side[i], name[i]);
				}
				lastupdate = clock();
				continue;
			}
			if (ChangeS.Print())ChangeSide(mykey);
			PackUserInfo(datareply);
		}
		else
		{
			for (int i = 0; i < cntu; i++)
			{
				Control(name[i], ctrl[i]);
			}
			int timetip = clock();
			UpdateWorld(timetip - lastupdate, CheckPlayerId(usrname));
			lastupdate = timetip;
			Vector campos = GetPlayerPos(CheckPlayerId(usrname));
			GetBorder(x1, y1, x2, y2);
			campos.x = max(campos.x, min(x1 + 800.0, (x1 + x2)*0.5));
			campos.x = min(campos.x, max(x2 - 800.0, (x1 + x2)*0.5));
			campos.y = max(campos.y, min(y1 + 450.0, (y1 + y2)*0.5));
			campos.y = min(campos.y, max(y2 - 450.0, (y1 + y2)*0.5));
			PrintWorld(campos.x, campos.y, CheckPlayerId(usrname));
			PackWorldInfo(datareply);
			sprintf(str, "FPS:%d", FPS());
			PutStringH(str, 10, 10, getheight()*0.05, 0);
		}
		Broadcast(&srv, datareply.buf, datareply.len);
		KickUser();
		POINT mpos; GetPos(&mpos);
		PrintLine(mpos.x - 20, mpos.y, mpos.x + 20, mpos.y, 0xFF8888);
		PrintLine(mpos.x, mpos.y - 20, mpos.x, mpos.y + 20, 0xFF8888);
		PrintFan(mpos.x, mpos.y, 0.0, 2 * PI, 15, 16, 0xFF8888);
		FlushBatchDraw();
		SLEEP(10);
	}
	StopServer(&srv);
}
void Game04Client(char *ip, char *usrname)
{
	cleardevice();
	FlushBatchDraw();
	InitSocket();
	Connect(&cln, ip, 62503);
	if (cln.clnsock == SOCKET_ERROR)return;
	string uname = usrname;
	ResetSendbag(&sendbag);
	AddInfo(&sendbag, "REGISTER", 8);
	AddString(&sendbag, uname.c_str(), uname.length());
	SendMessage(cln.clnsock, sendbag.buf, sendbag.len);
	UpdateClient(&cln);
	CSNAPSHOT snapshot;
	Button Exit, ChangeS;
	Exit.SetButton("Exit", 0.02, 0.9, 0.3, 0.08);
	ChangeS.SetButton("ChangeSide", 0.02, 0.02, 0.3, 0.06);
	int cntside[2], lastupdate;
	double x1, y1, x2, y2;
	while (!GetAsyncKeyState(VK_ESCAPE))
	{
		cleardevice();
		UpdateClient(&cln);
		while (!cln.MessageQueueEmpty())
		{
			Message message = cln.TakeMessage();
			ProcessMessage(message, usrname);
		}
		TakeSnapshot(snapshot);
		if (mykey != -1)Control(usrname, snapshot);
		ResetSendbag(&sendbag);
		AddInfo(&sendbag, "SETCTROL", 8);
		AddLongLong(&sendbag, mykey);
		AddInfo(&sendbag, (char*)&snapshot, sizeof(CSNAPSHOT));
		SendMessage(cln.clnsock, sendbag.buf, sendbag.len);
		if (state == 0)
		{
			cntside[0] = 0;
			cntside[1] = 0;
			double x2, y2;
			x2 = getwidth()*0.4;
			y2 = getheight()*0.04;
			for (int i = 0; i < cntu; i++)
			{
				double x1, y1;
				x1 = 0.05;
				if (side[i])x1 += 0.5;
				x1 *= getwidth();
				y1 = 0.1;
				y1 += 0.05*cntside[side[i]]++;
				y1 *= getheight();
				PrintRectangle(x1, y1, x1 + x2, y1 + y2, side[i] ? 0xFF0000 : 0xFF, 0.3);
				PutStringH(name[i].c_str(), x1, y1, y2, key[i] == mykey ? 0xFF : 0);
			}
			if (Exit.Print())break;
			if (ChangeS.Print())
			{
				ResetSendbag(&sendbag);
				AddInfo(&sendbag, "CHANGESD", 8);
				AddLongLong(&sendbag, mykey);
				SendMessage(cln.clnsock, sendbag.buf, sendbag.len);
			}
		}
		else
		{
			for (int i = 0; i < cntu; i++)
			{
				Control(name[i], ctrl[i]);
			}
			int timetip = clock();
			//UpdateWorld(timetip - lastupdate, CheckPlayerId(usrname));
			lastupdate = timetip;//*/
			Vector campos;
			if (mykey != -1)campos = GetPlayerPos(CheckPlayerId(usrname));
			GetBorder(x1, y1, x2, y2);
			campos.x = max(campos.x, min(x1 + 800.0, (x1 + x2)*0.5));
			campos.x = min(campos.x, max(x2 - 800.0, (x1 + x2)*0.5));
			campos.y = max(campos.y, min(y1 + 450.0, (y1 + y2)*0.5));
			campos.y = min(campos.y, max(y2 - 450.0, (y1 + y2)*0.5));
			if (mykey != -1)PrintWorld(campos.x, campos.y, CheckPlayerId(usrname));
			else PrintWorld(campos.x, campos.y, -1);
			sprintf(str, "FPS:%d", FPS());
			PutStringH(str, 10, 10, getheight()*0.05, 0);
		}
		POINT mpos; GetPos(&mpos);
		PrintLine(mpos.x - 20, mpos.y, mpos.x + 20, mpos.y, 0xFF8888);
		PrintLine(mpos.x, mpos.y - 20, mpos.x, mpos.y + 20, 0xFF8888);
		PrintFan(mpos.x, mpos.y, 0.0, 2 * PI, 15, 16, 0xFF8888);
		FlushBatchDraw();
		SLEEP(10);
	}
	closesocket(cln.clnsock);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int cmdShow)
{
	CreateWin();
	InitRandomSystem();
	setbkmode(TRANSPARENT);
	setbkcolor(0xFFFFFF);
	BeginBatchDraw();
	CreateWorld();
	Button CreateServer, JoinServer, Exit;
	Inputbox IPaddr,NameBlank;
	NameBlank.SetInputbox(0.1, 0.1, 0.4, 0.08);
	CreateServer.SetButton("Create Server", 0.1, 0.2, 0.8, 0.08);
	IPaddr.SetInputbox(0.1, 0.3, 0.8, 0.08);
	JoinServer.SetButton("Join Server", 0.1, 0.4, 0.8, 0.08);
	Exit.SetButton("Exit", 0.1, 0.5, 0.8, 0.08);
	char ip[128], name[128];
	while (true)
	{
		cleardevice();
		if (CreateServer.Print())
		{
			memset(name, 0, sizeof(name));
			NameBlank.GetMessage(name);
			Game04Server(name);
			NameBlank.SetInputbox(0.1, 0.1, 0.4, 0.08);
			CreateServer.SetButton("Create Server", 0.1, 0.2, 0.8, 0.08);
			IPaddr.SetInputbox(0.1, 0.3, 0.8, 0.08);
			JoinServer.SetButton("Join Server", 0.1, 0.4, 0.8, 0.08);
			Exit.SetButton("Exit", 0.1, 0.5, 0.8, 0.08);
			continue;
		}
		if (JoinServer.Print())
		{
			memset(ip, 0, sizeof(ip));
			memset(name, 0, sizeof(name));
			IPaddr.GetMessage(ip);
			NameBlank.GetMessage(name);
			Game04Client(ip, name);
			NameBlank.SetInputbox(0.1, 0.1, 0.4, 0.08);
			CreateServer.SetButton("Create Server", 0.1, 0.2, 0.8, 0.08);
			IPaddr.SetInputbox(0.1, 0.3, 0.8, 0.08);
			JoinServer.SetButton("Join Server", 0.1, 0.4, 0.8, 0.08);
			Exit.SetButton("Exit", 0.1, 0.5, 0.8, 0.08);
			continue;
		}
		if (Exit.Print())
		{
			break;
		}
		IPaddr.Print();
		NameBlank.Print();
		sprintf(str, "FPS:%d", FPS());
		PutStringH(str, 10, 10, getheight()*0.05, 0);
		POINT mpos; GetPos(&mpos);
		PrintLine(mpos.x - 20, mpos.y, mpos.x + 20, mpos.y, 0xFF8888);
		PrintLine(mpos.x, mpos.y - 20, mpos.x, mpos.y + 20, 0xFF8888);
		PrintFan(mpos.x, mpos.y, 0.0, 2 * PI, 15, 16, 0xFF8888);
		FlushBatchDraw();
		SLEEP(10);
	}
}
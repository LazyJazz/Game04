#pragma once
#include<iostream>
#include<cstring>
#include<cstdio>
#define CLIENT_LIMIT 128
#define FD_SETSIZE CLIENT_LIMIT
#define BAGSIZE 1048576
#include<Windows.h>
#include<queue>
#include<time.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
void InitSocket();

SOCKET CreateServer(int port);

struct CONNECTION
{
	SOCKET clnsock;
	SOCKADDR_IN clnaddr;
	int len,p,last_recv;
	char buf[BAGSIZE];
};
struct MESSAGE
{
	SOCKET clnsock;
	SOCKADDR_IN clnaddr;
	int len;
	char *buf;
};
struct Message
{
	SOCKET clnsock;
	SOCKADDR_IN clnaddr;
	int len;
	char buf[BAGSIZE];
};
struct SendBag
{
	int len;
	char buf[BAGSIZE];
};
struct SockServer
{
	SOCKET srvsock;
	CONNECTION connections[CLIENT_LIMIT];
	int cnt_connections;
	queue<MESSAGE> recv_que;
	bool MessageQueueEmpty()
	{
		return recv_que.empty();
	}
	Message PeekMessage()
	{
		Message ret;
		ret.clnsock=recv_que.front().clnsock;
		ret.clnaddr=recv_que.front().clnaddr;
		ret.len=recv_que.front().len;
		memcpy(ret.buf,recv_que.front().buf,ret.len+1); 
		return ret;
	}
	void PopMessage()
	{
		delete[]recv_que.front().buf;
		recv_que.pop();
	}
	Message TakeMessage()
	{
		Message ret;
		ret.clnsock=recv_que.front().clnsock;
		ret.clnaddr=recv_que.front().clnaddr;
		ret.len=recv_que.front().len;
		memcpy(ret.buf,recv_que.front().buf,ret.len+1); 
		PopMessage();
		return ret;
	}
};
void SendMessage(SOCKET sock, const char *message, int len);
void StartServer(SockServer *ss, int port);
void StopServer(SockServer *ss);
void UpdateServer(SockServer *ss);
void Broadcast(SockServer *ss, const char * message, int len);
struct CLIENT
{
	SOCKET clnsock;
	int p,len;
	char buf[BAGSIZE];
	queue<MESSAGE> recv_que;
	bool MessageQueueEmpty()
	{
		return recv_que.empty();
	}
	Message PeekMessage()
	{
		Message ret;
		ret.clnsock=recv_que.front().clnsock;
		ret.clnaddr=recv_que.front().clnaddr;
		ret.len=recv_que.front().len;
		memcpy(ret.buf,recv_que.front().buf,ret.len+1); 
		return ret;
	}
	void PopMessage()
	{
		delete[]recv_que.front().buf;
		recv_que.pop();
	}
	Message TakeMessage()
	{
		Message ret;
		ret.clnsock=recv_que.front().clnsock;
		ret.clnaddr=recv_que.front().clnaddr;
		ret.len=recv_que.front().len;
		memcpy(ret.buf,recv_que.front().buf,ret.len+1); 
		PopMessage();
		return ret;
	}
};
SOCKET CreateClient(const char * ip, int port);
void Connect(CLIENT *cln, const char * ip, int port);
void UpdateClient(CLIENT *cln);

void ResetSendbag(SendBag *sendbag);

void AddString(SendBag *sendbag, const char *str, int len);

void AddInfo(SendBag *sendbag, const char *str, int len);

void AddInt(SendBag *sendbag, int n);

void AddLongLong(SendBag *sendbag, long long n);

void AddDouble(SendBag *sendbag, double n);

int GetString(char *&p, char *ret);

void GetInfo(char *&p, int len, char * targ);

int GetInt(char *&p);

long long GetLongLong(char *&p);

double GetDouble(char *&p);


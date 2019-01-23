#include"SocketLib.h"

void InitSocket()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}
SOCKET CreateServer(int port)
{
	SOCKET ret;
	SOCKADDR_IN srvaddr;
	ret = socket(AF_INET, SOCK_STREAM, 0);
	srvaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(port);
	bind(ret, (SOCKADDR*)&srvaddr, sizeof(SOCKADDR));
	listen(ret, CLIENT_LIMIT);
	int nRecvBuf = CLIENT_LIMIT*BAGSIZE;
	setsockopt(ret, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	int nSendBuf = CLIENT_LIMIT*BAGSIZE;
	setsockopt(ret, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));
	return ret;
}
void SendMessage(SOCKET sock, const char *message, int len)
{
	send(sock, (char*)&len, sizeof(int), 0);
	send(sock, message, len, 0);
}
void StartServer(SockServer *ss, int port)
{
	ss->cnt_connections = 0;
	ss->srvsock = CreateServer(port);
}
void StopServer(SockServer *ss)
{
	for (int i = 0; i < ss->cnt_connections; i++)closesocket(ss->connections[i].clnsock);
	closesocket(ss->srvsock);
}
char recv_buf[BAGSIZE];
void UpdateServer(SockServer *ss)
{
	int timetip = clock();
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(ss->srvsock, &rset);
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;
	select(0, &rset, NULL, NULL, &timeout);
	int addrlen = sizeof(SOCKADDR);
	if (FD_ISSET(ss->srvsock, &rset))
	{
		SOCKADDR_IN new_clnaddr;
		SOCKET new_cln = accept(ss->srvsock, (SOCKADDR FAR*)&new_clnaddr, &addrlen);
		if (new_cln != INVALID_SOCKET)
		{
			if (ss->cnt_connections<CLIENT_LIMIT)
			{
				ss->connections[ss->cnt_connections].clnsock = new_cln;
				ss->connections[ss->cnt_connections].clnaddr = new_clnaddr;
				ss->connections[ss->cnt_connections].last_recv = timetip;
				ss->connections[ss->cnt_connections].len = 0;
				ss->connections[ss->cnt_connections].p = 0;
				memset(ss->connections[ss->cnt_connections].buf, 0, sizeof(ss->connections[ss->cnt_connections].buf));
				ss->cnt_connections++;
			}
			else closesocket(new_cln);
		}
		else closesocket(new_cln);
	}
	FD_ZERO(&rset);
	for (int i = 0; i<ss->cnt_connections; i++)FD_SET(ss->connections[i].clnsock, &rset);
	select(0, &rset, NULL, NULL, &timeout);
	for (int i = 0; i<ss->cnt_connections; i++)
	{
		CONNECTION *pre = &(ss->connections[i]);
		if (FD_ISSET(pre->clnsock, &rset))
		{
			memset(recv_buf, 0, sizeof(recv_buf));
			int recv_len = recv(pre->clnsock, recv_buf, sizeof(recv_buf), 0);
			if (recv_len>0)
			{
				pre->last_recv = timetip;
				for (int i = 0; i<recv_len; i++)
				{
					int c = recv_buf[i];
					if (pre->len)
					{
						pre->buf[pre->p++] = c;
						if (pre->p == pre->len)
						{
							MESSAGE message;
							message.clnsock = pre->clnsock;
							message.clnaddr = pre->clnaddr;
							message.len = pre->len;
							message.buf = new char[pre->len + 1];
							memcpy(message.buf, pre->buf, pre->len + 1);
							memset(pre->buf, 0, pre->len);
							pre->len = 0;
							pre->p = 0;
							ss->recv_que.push(message);
						}
					}
					else
					{
						pre->buf[pre->p++] = c;
						if (pre->p == 4)
						{
							pre->len = (int)pre->buf[0] + (int)pre->buf[1] * 256 + (int)pre->buf[2] * 256 * 256 + (int)pre->buf[3] * 256 * 256 * 256;
							pre->p = 0;
							memset(pre->buf, 0, 4);
						}
					}
				}
			}
		}
	}
	int cnt_last = 0;
	for (int i = 0; i<ss->cnt_connections; i++)
	{
		if (timetip - ss->connections[i].last_recv <= 3000)
		{
			ss->connections[cnt_last++] = ss->connections[i];
		}
		else
		{
			closesocket(ss->connections[i].clnsock);
		}
	}
	ss->cnt_connections = cnt_last;
	/*printf("Alive connections: %d\n",ss->cnt_connections);
	for(int i=0;i<ss->cnt_connections;i++)
	{
	printf("CONNECTION #%d ip:%s last_recv:%d\n",i,inet_ntoa(ss->connections[i].clnaddr.sin_addr),ss->connections[i].last_recv);
	}//*/
}
void Broadcast(SockServer * ss, const char * message, int len)
{
	for (int i = 0; i < ss->cnt_connections; i++)
	{
		SendMessage(ss->connections[i].clnsock, message, len);
	}
}
SOCKET CreateClient(const char * ip, int port)
{
	SOCKADDR_IN srvaddr;
	SOCKET client = socket(AF_INET, SOCK_STREAM, 0);
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = inet_addr(ip);
	srvaddr.sin_port = htons(port);
	int addrlen = sizeof(SOCKADDR);
	int nRecvBuf = CLIENT_LIMIT*BAGSIZE;
	setsockopt(client, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	int nSendBuf = CLIENT_LIMIT*BAGSIZE;
	setsockopt(client, SOL_SOCKET, SO_SNDBUF, (const char*)&nSendBuf, sizeof(int));
	int ret;
	int start_time = clock();
	while ((ret = connect(client, (LPSOCKADDR)&srvaddr, addrlen)) != SOCKET_ERROR)
	{
		fd_set wset;
		FD_ZERO(&wset);
		FD_SET(client, &wset);
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		select(0, NULL, &wset, NULL, &timeout);
		if (FD_ISSET(client, &wset))break;
		if (clock() - start_time > 3000)return SOCKET_ERROR;
	}
	if (ret == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	return client;
}
void Connect(CLIENT *cln, const char * ip, int port)
{
	cln->clnsock = CreateClient(ip, port);
	cln->len = 0;
	cln->p = 0;
	memset(cln->buf, 0, sizeof(cln->buf));
}
fd_set rset;
void UpdateClient(CLIENT *cln)
{
	int timetip = clock();
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;
	FD_ZERO(&rset);
	FD_SET(cln->clnsock, &rset);
	select(0, &rset, NULL, NULL, &timeout);
	if (FD_ISSET(cln->clnsock, &rset))
	{
		
		memset(recv_buf, 0, sizeof(recv_buf));
		int ret = recv(cln->clnsock, recv_buf, sizeof(recv_buf), 0);
		if (ret>0)
		{
			for (int i = 0; i<ret; i++)
			{
				int c = recv_buf[i];
				if (cln->len)
				{
					cln->buf[cln->p++] = c;
					if (cln->p == cln->len)
					{
						MESSAGE message;
						message.clnsock = cln->clnsock;
						message.len = cln->len;
						message.buf = new char[cln->len + 1];
						memcpy(message.buf, cln->buf, cln->len + 1);
						cln->recv_que.push(message);
						cln->p = 0;
						cln->len = 0;
						memset(cln->buf, 0, cln->len + 1);
					}
				}
				else
				{
					cln->buf[cln->p++] = c;
					if (cln->p == 4)
					{
						cln->len = *(int*)cln->buf;
						//cln->len=(int)cln->buf[0]+(int)cln->buf[1]*256+(int)cln->buf[2]*256*256+(int)cln->buf[3]*256*256*256;
						cln->p = 0;
						memset(cln->buf, 0, 4);
					}
				}
			}
		}
	}
}

void ResetSendbag(SendBag *sendbag)
{
	memset(sendbag->buf, 0, sizeof(sendbag->buf));
	sendbag->len = 0;
}

void AddString(SendBag * sendbag, const char * str, int len)
{
	*((int*)(sendbag->buf + sendbag->len)) = len;
	sendbag->len += 4;
	for (int i = 0; i < len; i++)sendbag->buf[sendbag->len++] = str[i];
}

void AddInfo(SendBag * sendbag, const char * str, int len)
{
	for (int i = 0; i < len; i++)sendbag->buf[sendbag->len++] = str[i];
}

void AddInt(SendBag * sendbag, int n)
{
	AddInfo(sendbag, (char*)&n, sizeof(int));
}

void AddLongLong(SendBag * sendbag, long long n)
{
	AddInfo(sendbag, (char*)&n, sizeof(long long));
}

void AddDouble(SendBag * sendbag, double n)
{
	AddInfo(sendbag, (char*)&n, sizeof(double));
}

int GetString(char *& p, char * ret)
{
	int len = *((int*)p);
	p += sizeof(int);
	for (int i = 0; i < len; i++)ret[i] = p[i];
	p += len;
	ret[len] = 0;
	return len;
}

void GetInfo(char *& p, int len, char * targ)
{
	memcpy(targ, p, len);
	p += len;
}

int GetInt(char *& p)
{
	int ret = *((int*)p);
	p += sizeof(int);
	return ret;
}

long long GetLongLong(char *& p)
{
	long long ret = *((long long*)p);
	p += sizeof(long long);
	return ret;
}

double GetDouble(char *& p)
{
	double ret = *((double*)p);
	p += sizeof(double);
	return ret;
}


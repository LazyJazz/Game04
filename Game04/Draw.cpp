#include"Draw.h"
char c_buffer[110];
TCHAR t_buffer[110];
DWORD *window_buffer;
int ScrX, ScrY;
HIMC hIMC_;
void onPasswordFocusChanged(bool isfocus)
{
	HWND wnd = GetHWnd();
	if (isfocus)
	{
		//focus on        
		hIMC_ = ::ImmGetContext(wnd);
		if (hIMC_)
		{
			ImmAssociateContext(wnd, (HIMC)nullptr);
			ImmReleaseContext(wnd, hIMC_);
		}
		return;
	}
	else
	{
		//focus out  
		if (hIMC_)
		{
			ImmDestroyContext(hIMC_);
			hIMC_ = (HIMC)nullptr;
		}

		hIMC_ = ImmCreateContext();
		if (hIMC_)
		{
			ImmAssociateContext(wnd, hIMC_);
			ImmReleaseContext(wnd, hIMC_);
		}
	}
}
void CreateWin()
{
	
	//initgraph(ScrX = 800, ScrY = 600);/*
	ScrX = GetSystemMetrics(SM_CXSCREEN), ScrY = GetSystemMetrics(SM_CYSCREEN);
	initgraph(ScrX, ScrY);
	HWND hwnd = GetHWnd();
	SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) - WS_CAPTION);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, ScrX, ScrY, SWP_SHOWWINDOW);//*/

	onPasswordFocusChanged(1);

	//HCURSOR hcur = LoadCursor(NULL, IDC_CROSS);
	HMODULE hmod = GetModuleHandle(NULL); // 获取当前进程的句柄
	HCURSOR hcur = LoadCursor(hmod,MAKEINTRESOURCE(IDC_CURSOR1)); // 加载资源中的鼠标样式图片
	SetClassLongPtr(hwnd, -12, (long)hcur);

	window_buffer = GetImageBuffer();//*/
}

char * TcharToChar(const TCHAR * tchar)
{
	TcharToChar(tchar, c_buffer);
	return c_buffer;
}

TCHAR * CharToTchar(const char * _char)
{
	CharToTchar(_char, t_buffer);
	return t_buffer;
}

void PutString(const char * str, int x, int y, int color)
{
	if(color==-1)outtextxy(x, y, CharToTchar(str));
	else
	{
		int backup_color = gettextcolor();
		settextcolor(color);
		outtextxy(x, y, CharToTchar(str));
		settextcolor(backup_color);
	}
}

void PutStringH(const char * str, int x, int y, int height, int color)
{
	SetTextStyle(height, "微软雅黑");
	PutString(str, x, y, color);
}

void SetTextStyle(int height,const char * font)
{
	settextstyle(height, 0, CharToTchar(font));
}

void MixPixel(int p, int color, double a)
{
	if (a <= 0.0)return;
	a = min(a, 1);
	int R = GetRValue(window_buffer[p]), G = GetGValue(window_buffer[p]), B = GetBValue(window_buffer[p]);
	R = R*(1.0 - a) + GetRValue(color)*a;
	G = G*(1.0 - a) + GetGValue(color)*a;
	B = B*(1.0 - a) + GetBValue(color)*a;
	window_buffer[p] = RGB(R, G, B);
}

int MixColor(int color1, int color2, double a)
{
	int R = GetRValue(color1), G = GetGValue(color1), B = GetBValue(color1);
	R = R*(1.0 - a) + GetRValue(color2)*a;
	G = G*(1.0 - a) + GetGValue(color2)*a;
	B = B*(1.0 - a) + GetBValue(color2)*a;
	return RGB(R, G, B);
}

void PrintCircle(double x, double y, double radius, int color)
{
	int l = x - radius, u = y - radius, r = x + radius + 1, d = y + radius + 1;
	double dist, sqr_r = radius*radius, sqr_r_1 = sqr_r + 2 * radius + 1;
	l = max(l, 0);
	r = min(r, ScrX - 1);
	u = max(u, 0);
	d = min(d, ScrY - 1);
	for (int j = u; j <= d; j++)
	{
		for (int i = l; i <= r; i++)
		{
			dist = (i - x)*(i - x) + (j - y)*(j - y);
			if (dist <= sqr_r)window_buffer[j*ScrX + i] = color;
			else if (dist <= sqr_r_1) MixPixel(j*ScrX + i, color, 1.0 - sqrt(dist) + radius);
		}
	}
}

double MinDist(double x, double y, double x1, double y1, double x2, double y2)
{
	if (x1 > x2)swap(x1, x2);
	if (y1 > y2)swap(y1, y2);
	double rx, ry;
	if (x < x1)rx = x1 - x;
	else if (x < x2)rx = 0.0;
	else rx = x - x2;
	if (y < y1)ry = y1 - y;
	else if (y < y2)ry = 0.0;
	else ry = y - y2;
	return sqrt(rx*rx + ry*ry);
}

void PrintRectangle(double l, double u, double r, double d, int color)
{
	if (l > r)swap(l, r);
	if (u > d)swap(u, d);
	double L = l, U = u, R = r + 1, D = d + 1;
	L = max(L, 0);
	U = max(U, 0);
	R = min(R, ScrX - 1);
	D = min(D, ScrY - 1);
	for (int j = U; j <= D; j++)
	{
		for (int i = L; i <= R; i++)
		{
			if (i > l&&i<r&&j>u&&j < d)window_buffer[j*ScrX + i] = color;
			else MixPixel(j*ScrX + i, color, 1.0 - MinDist(i, j, l, u, r, d));
		}
	}
}

void PrintLine(double x1, double y1, double x2, double y2, int color, double width)
{
	bool rev = 0;
	if (abs(x1 - x2) < abs(y1 - y2))swap(x1, y1), swap(x2, y2), rev = 1;
	if (x1 > x2)swap(x1, x2), swap(y1, y2);
	//PrintCircle(x1, y1, width*0.5, color);
	//PrintCircle(x2, y2, width*0.5, color);
	Vector v1(x1, y1), v2(x2, y2), di = v2 - v1, p1, p2, p3, p4;
	di = di.Vertical();
	di = di.Zoom(width*0.5);
	p1 = v1 + di;
	p2 = v2 + di;
	p3 = v1 - di;
	p4 = v2 - di;
	/*
	p1--------------------p2
	v1                    v2
	p3--------------------p4
	*/
	Line L, U, R, D;
	L = Line(p1, p3); L.FixDirection(v2);
	R = Line(p2, p4); R.FixDirection(v1);
	U = Line(p1, p2); U.FixDirection((p3 + p4)*0.5);
	D = Line(p3, p4); D.FixDirection((p1 + p2)*0.5);
	int l = min(p1.x, min(p2.x, min(p3.x, p4.x))) - 1 - width*0.5, r = max(p1.x, max(p2.x, max(p3.x, p4.x))) + 1 + width*0.5;
	l = max(l, 0);
	if (rev)r = min(r, ScrY - 1);
	else r = min(r, ScrX - 1);
	for (int i = l; i <= r; i++)
	{
		int u, d; Vector i1, i2; Line V(Vector(i, 0), Vector(i, 1));
		i1 = V&&U;
		i2 = V&&D;
		u = min(i1.y, i2.y) - 1 - width*0.5;
		d = max(i1.y, i2.y) + 1 ;
		u = max(u, 0);
		if (rev)d = min(d, ScrX - 1);
		else d = min(d, ScrY - 1);
		for (int j = u; j <= d; j++)
		{
			i1 = Vector(i, j);
			double d = min(L.Check(i1), min(U.Check(i1), min(R.Check(i1), D.Check(i1))));
			int p = j*ScrX + i;
			if (rev)p = i*ScrX + j;
			i2 = v1 - i1;
			d = max(d, width*0.5 - i2.Dist());
			i2 = v2 - i1;
			d = max(d, width*0.5 - i2.Dist());
			if(d>=0.0)window_buffer[p] = color;
			else MixPixel(p, color, 1.0+d);//*/
		}
	}
}

void PrintHalo(double x, double y, double radius, int color)
{
	int l = x - radius, u = y - radius, r = x + radius + 1, d = y + radius + 1;
	double dist, sqr_r = radius*radius, sqr_r_1 = sqr_r + 2 * radius + 1;
	l = max(l, 0);
	r = min(r, ScrX - 1);
	u = max(u, 0);
	d = min(d, ScrY - 1);
	for (int j = u; j <= d; j++)
	{
		for (int i = l; i <= r; i++)
		{
			dist = (i - x)*(i - x) + (j - y)*(j - y);
			dist /= sqr_r;
			MixPixel(j*ScrX + i, color, 1.0 - dist);
		}
	}
}

void PrintCircle(double x, double y, double radius, int color, double alpha)
{
	int l = x - radius, u = y - radius, r = x + radius + 1, d = y + radius + 1;
	double dist, sqr_r = radius*radius, sqr_r_1 = sqr_r + 2 * radius + 1;
	l = max(l, 0);
	r = min(r, ScrX - 1);
	u = max(u, 0);
	d = min(d, ScrY - 1);
	for (int j = u; j <= d; j++)
	{
		for (int i = l; i <= r; i++)
		{
			dist = (i - x)*(i - x) + (j - y)*(j - y);
			if (dist <= sqr_r)MixPixel(j*ScrX + i, color, alpha);
			else if (dist <= sqr_r_1) MixPixel(j*ScrX + i, color, (1.0 - sqrt(dist) + radius)*alpha);
		}
	}
}

void PrintRectangle(double l, double u, double r, double d, int color, double alpha)
{
	if (l > r)swap(l, r);
	if (u > d)swap(u, d);
	double L = l, U = u, R = r + 1, D = d + 1;
	L = max(L, 0);
	U = max(U, 0);
	R = min(R, ScrX - 1);
	D = min(D, ScrY - 1);
	for (int j = U; j <= D; j++)
	{
		for (int i = L; i <= R; i++)
		{
			if (i > l&&i<r&&j>u&&j < d)MixPixel(j*ScrX + i, color, alpha);
			else MixPixel(j*ScrX + i, color, (1.0 - MinDist(i, j, l, u, r, d))*alpha);
		}
	}
}

void PutStringMid(const char * str, int x, int y)
{
	CharToTchar(str, t_buffer);
	x = x - textwidth(t_buffer) / 2;
	y = y - textheight(t_buffer) / 2;
	outtextxy(x, y, t_buffer);
}

void PutStringMidH(const char * str, int x, int y, int height, int color)
{
	SetTextStyle(height, "微软雅黑");
	int backup = gettextcolor();
	settextcolor(color);
	PutStringMid(str, x, y);
	settextcolor(backup);
}

double Camx, Camy;
double scalex, scaley, scale;
void SetCam(double camx, double camy)
{
	scalex = getwidth() / 1600.0, scaley = getheight() / 900.0, scale = min(scalex, scaley);
	double midx = getwidth()*0.5 - 0.5, midy = getheight()*0.5 - 0.5;
	Camx = camx*scale;
	Camy = camy*scale;
	Camx -= midx;
	Camy -= midy;
}

void Transform(double &x, double &y)
{
	x *= scale;
	y *= scale;
	x -= Camx;
	y = Camy - y + getheight();
}

void RevTransform(double &x, double &y)
{
	x += Camx;
	y = -y + Camy + getheight();
	x /= scale;
	y /= scale;
}

void PrintRectangleScale(double x1, double y1, double x2, double y2, int color, double alpha)
{
	Transform(x1, y1);
	Transform(x2, y2);
	if (alpha >= 1.0)PrintRectangle(x1, y1, x2, y2, color);
	else if (alpha < 0.0)return;
	else PrintRectangle(x1, y1, x2, y2, color, alpha);
}

void PrintCircleScale(double x, double y, double r, int color, double alpha)
{
	Transform(x, y);
	r *= scale;
	if (alpha >= 1.0)PrintCircle(x, y, r, color);
	else if (alpha < 0.0)return;
	else PrintCircle(x, y, r, color, alpha);
}

void PrintLineScale(double x1, double y1, double x2, double y2, int color, double width)
{
	Transform(x1, y1);
	Transform(x2, y2);
	PrintLine(x1, y1, x2, y2, color, width*scale);
}

void PrintHaloScale(double x, double y, double r, int color)
{
	Transform(x, y);
	r *= scale;
	PrintHalo(x, y, r, color);
}

void PrintFan(double x, double y, double ds, double dp, double r1, double r2, int color, double alpha)
{
	ds -= ((int)(ds / 2 / PI))*(2 * PI);
	if (r1 > r2)swap(r1, r2);
	int x1, y1, x2, y2;
	double dx, dy, dist, cx, cy;
	double d;
	x1 = x - r2 - 1;
	y1 = y - r2 - 1;
	x2 = x + r2 + 1;
	y2 = y + r2 + 2;
	x1 = max(x1, 0);
	y1 = max(y1, 0);
	x2 = min(x2, ScrX - 1);
	y2 = min(y2, ScrY - 1);
	for (int j = y1; j < y2; j++)
	{
		dy = j - y;
		double l, r, lin, rin;
		if (abs(dy) <= r2 + 1.0)
		{
			l = x - sqrt(r2*r2 + 2 * r2 + 1.0 - dy*dy);
			r = 2 * x - l;
			l = max(l, x1);
			r = min(r, x2);
			if (abs(dy) <= r1 - 1.0)
			{
				lin=x- sqrt(r1*r1 - 2 * r1 + 1.0 - dy*dy);
				rin = 2 * x - lin;
				lin = min(lin, x2);
				rin = max(rin, x1);
				for (int i = l; i <= lin; i++)
				{
					dx = i - x;
					d = atan2(dx, dy);
					dist = sqrt(dx*dx + dy*dy);
					if (dist < r1)cy = r1 - dist;
					else if (dist < r2) cy = 0.0;
					else cy = dist - r2;
					d -= ds;
					while (d < 0.0)d += 2 * PI;
					while (d >= 2 * PI)d -= 2 * PI;
					if (d < dp)cx = 0.0;
					else
					{
						d = min(d - dp, 2 * PI - d);
						cx = d*dist;
					}
					d = 1.0 - sqrt(cx*cx + cy*cy);
					d *= alpha;
					d = max(d, 0);
					if (d == 1.0)window_buffer[j*ScrX + i] = color;
					else MixPixel(j*ScrX + i, color, d);
				}
				for (int i = rin; i <= r; i++)
				{
					dx = i - x;
					d = atan2(dx, dy);
					dist = sqrt(dx*dx + dy*dy);
					if (dist < r1)cy = r1 - dist;
					else if (dist < r2) cy = 0.0;
					else cy = dist - r2;
					d -= ds;
					while (d < 0.0)d += 2 * PI;
					while (d >= 2 * PI)d -= 2 * PI;
					if (d < dp)cx = 0.0;
					else
					{
						d = min(d - dp, 2 * PI - d);
						cx = d*dist;
					}
					d = 1.0 - sqrt(cx*cx + cy*cy);
					d *= alpha;
					d = max(d, 0);
					if (d == 1.0)window_buffer[j*ScrX + i] = color;
					else MixPixel(j*ScrX + i, color, d);
				}
			}
			else
				for (int i = l; i <= r; i++)
				{
					dx = i - x;
					d = atan2(dx, dy);
					dist = sqrt(dx*dx + dy*dy);
					if (dist < r1)cy = r1 - dist;
					else if (dist < r2) cy = 0.0;
					else cy = dist - r2;
					d -= ds;
					while (d < 0.0)d += 2 * PI;
					while (d >= 2 * PI)d -= 2 * PI;
					if (d < dp)cx = 0.0;
					else
					{
						d = min(d - dp, 2 * PI - d);
						cx = d*dist;
					}
					d = 1.0 - sqrt(cx*cx + cy*cy);
					d *= alpha;
					d = max(d, 0);
					if (d == 1.0)window_buffer[j*ScrX + i] = color;
					else MixPixel(j*ScrX + i, color, d);
				}
		}
	}
}

void PrintFanScale(double x, double y, double ds, double dp, double r1, double r2, int color, double alpha)
{
	Transform(x, y);
	r1 *= scale;
	r2 *= scale;
	PrintFan(x, y, ds, dp, r1, r2, color, alpha);
}

double GetScale()
{
	return scale;
}

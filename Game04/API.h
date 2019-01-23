#include"functions.h"
#include<conio.h>
void GetPos(LPPOINT p);

void SLEEP(int t);

struct Button
{
private:
	int x, y, h, w;
	bool locked;
	char message[100];
public:
	void SetButton(char *s, int posx, int posy, int width, int height)
	{
		sprintf(message, "%s", s);
		x = posx;
		y = posy;
		h = height;
		w = width;
		locked = 0;
	}
	void SetButton(char *s, double posx, double posy, double width, double height)
	{
		sprintf(message, "%s", s);
		x = posx*getwidth();
		y = posy*getheight();
		h = height*getheight();
		w = width*getwidth();
		locked = 0;
	}
	bool Print()
	{
		bool ret = 0;
		POINT p;
		TextStyle("Î¢ÈíÑÅºÚ", h * 4 / 5);
		GetPos(&p);
		if (p.x >= x&&p.x <= x + w&&p.y >= y&&p.y <= h + y)
		{
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				locked = 1;
				SetColor(RGB(95, 95, 255));
			}
			else
			{
				if (locked)ret = 1;
				locked = 0;
				SetColor(RGB(127, 127, 255));
			}
		}
		else
		{
			locked = 0;
			SetColor(RGB(127, 127, 127));
		}
		fillrectangle(x, y, x + w, y + h);
		SetColor(RGB(63, 63, 63));
		TextXY(message, x + w / 2 - TextWidth(message) / 2, y + h / 2 - TextHeight(message) / 2);
		return ret;
	}
};
struct Inputbox
{
private:
	int x, y, h, w, len;
	bool locked,chose;
	char message[1000];
	bool key[256];
	int GetChar()
	{
		bool shifted = GetAsyncKeyState(VK_SHIFT);
		for (int i = 0; i < 10; i++)
		{
			if (GetAsyncKeyState(i + 48))
			{
				if (!key[i + 48])
				{
					key[i + 48] = 1;
					return '0' + i;
				}
			}
			else key[i + 48] = 0;
		}
		for (int i = 0; i < 26; i++)
		{
			if (GetAsyncKeyState(i + 65))
			{
				if (!key[i + 65])
				{
					key[i + 65] = 1;
					return (shifted ? 'A' : 'a') + i;
				}
			}
			else key[i + 65] = 0;
		}
		if (GetAsyncKeyState(VK_DECIMAL))
		{
			if (!key[VK_DECIMAL])
			{
				key[VK_DECIMAL] = 1;
				return '.';
			}
		}
		else key[VK_DECIMAL] = 0;
		if (GetAsyncKeyState(190))
		{
			if (!key[190])
			{
				key[190] = 1;
				return '.';
			}
		}
		else key[190] = 0;
		if (GetAsyncKeyState(VK_BACK))
		{
			if (!key[VK_BACK])
			{
				key[VK_BACK] = 1;
				return 8;
			}
		}
		else key[VK_BACK] = 0;
		if (GetAsyncKeyState(VK_SPACE))
		{
			if (!key[VK_SPACE])
			{
				key[VK_SPACE] = 1;
				return ' ';
			}
		}
		else key[VK_SPACE] = 0;
		return 0;
	}
public:
	void SetInputbox(int posx, int posy, int width, int height)
	{
		x = posx;
		y = posy;
		h = height;
		w = width;
		locked = 0;
		chose = 0;
		len = 0;
		memset(message, 0, sizeof(message));
		memset(key, 0, sizeof(key));
	}
	void SetInputbox(double posx, double posy, double width, double height)
	{
		x = posx*getwidth();
		y = posy*getheight();
		h = height*getheight();
		w = width*getwidth();
		locked = 0;
		chose = 0;
		len = 0;
		memset(message, 0, sizeof(message));
		memset(key, 0, sizeof(key));
	}
	void Print()
	{
		POINT p;
		TextStyle("Î¢ÈíÑÅºÚ", h * 4 / 5);
		GetPos(&p);
		if (p.x >= x&&p.x <= x + w&&p.y >= y&&p.y <= h + y)
		{
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				locked = 1;
			}
			else
			{
				if (locked)chose = 1;
				locked = 0;
			}
			SetColor(RGB(127, 127, 255));
		}
		else
		{
			if (GetAsyncKeyState(VK_LBUTTON))chose = 0;
			locked = 0;
			SetColor(RGB(127, 127, 127));
		}
		if (chose)
		{
			SetColor(RGB(95, 95, 255));
			int c = GetChar();
			if (c)
			{
				if (c == 8)
				{
					if (len)
					{
						message[len-1]=0;
						len--;
					}
					else
						Beep(1000, 100);
				}
				else
				{
					message[len++]=c;
					if (TextWidth(message) > w - 10)
					{
						message[len - 1] = 0;
						len--;
						Beep(1000,100);
					}
				}
			}
		}
		else
		{
			memset(key,1,sizeof(key));
		}
		fillrectangle(x, y, x + w, y + h);
		SetColor(RGB(63, 63, 63));
		TextXY(message, x + w / 2 - TextWidth(message) / 2, y + h / 2 - TextHeight(message) / 2);
	}
	int GetMessage(char *s)
	{
		sprintf(s,"%s",message);
		return len;
	}
};
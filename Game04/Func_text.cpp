#include"functions.h"
int Line = 0;
void TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度    
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char      
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}
void Text(char *s)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	outtextxy(10, 10, tchar);
}
void TextStyle(char *font, int height)
{
	TCHAR  tchar[1000];
	CharToTchar(font, tchar);
	settextstyle(height, 0, tchar);
}
int TextHeight(char* s)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	return textheight(tchar);
}
int TextWidth(char* s)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	return textwidth(tchar);
}
void TextMid(char *s)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	int x=getwidth()-textwidth(tchar),y=getheight()-textheight(tchar);
	x >>= 1, y >>= 1;
	outtextxy(x,y,tchar);
}
void SetColor(COLORREF c)
{
	setcolor(c);
	setfillcolor(c);
}
void TextXY(char *s, int x, int y)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	outtextxy(x, y, tchar);
}
void TextXY(char *s, double x, double y)
{
	TCHAR  tchar[1000];
	CharToTchar(s, tchar);
	outtextxy(x*getwidth(), y*getheight(), tchar);
}
void TextLine(char *s)
{
	TextXY(s,10,Line+10);
	Line += TextHeight(s)+10;
	Line++;
}
void ResetLine()
{
	Line=0;
}
int FPS()
{
	static queue<int> frame_time;
	int timetip = clock();
	frame_time.push(timetip);
	while (timetip - frame_time.front() >= 1000)frame_time.pop();
	return frame_time.size();
}

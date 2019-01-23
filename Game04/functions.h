#include<windows.h>
#include<ctime>
#include<graphics.h>
#include<cstdio>
#include<cmath>
#include<queue>
using namespace std;
void CharToTchar(const char * _char, TCHAR * tchar);

void TcharToChar(const TCHAR * tchar, char * _char);

void Text(char *s);

void TextStyle(char *font, int height);

int TextHeight(char *s);

int TextWidth(char *s);

void SetColor(COLORREF c);

void TextMid(char *s);

void TextXY(char *s, int x, int y);

void TextXY(char *s, double x, double y);

void TextLine(char *s);

void ResetLine();

int FPS();

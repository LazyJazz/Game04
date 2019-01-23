#include"API.h"
void GetPos(LPPOINT p)
{
	HWND hWnd = GetHWnd();
	GetCursorPos(p);   // 获取鼠标指针位置（屏幕坐标）
	ScreenToClient(hWnd, p);
}
void SLEEP(int t)
{
	static int c=clock();
	while (clock() < c + t);
	c = clock();
}
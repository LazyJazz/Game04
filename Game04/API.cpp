#include"API.h"
void GetPos(LPPOINT p)
{
	HWND hWnd = GetHWnd();
	GetCursorPos(p);   // ��ȡ���ָ��λ�ã���Ļ���꣩
	ScreenToClient(hWnd, p);
}
void SLEEP(int t)
{
	static int c=clock();
	while (clock() < c + t);
	c = clock();
}
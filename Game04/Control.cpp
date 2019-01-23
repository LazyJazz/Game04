#include "Control.h"

void TakeSnapshot(CSNAPSHOT & snapshot)
{
	snapshot.time = clock();
	for (int i = 0; i < 256; i++)snapshot.press[i] = GetAsyncKeyState(i);
	POINT mp;
	GetCursorPos(&mp);
	ScreenToClient(GetHWnd(), &mp);
	snapshot.mousepos.x = mp.x;
	snapshot.mousepos.y = mp.y;
	RevTransform(snapshot.mousepos.x, snapshot.mousepos.y);
}

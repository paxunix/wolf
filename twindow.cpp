/*
	TWINDOW.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TWindow class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TWindow::TWindow(PTWindowObject pParent, LPCSTR lpszTitle, int nID) : TWindowObject(pParent, lpszTitle, nID)
{
   	WndClass.style = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW | CS_BYTEALIGNWINDOW;	//default window style
	WndClass.lpfnWndProc = TWindow::StandardWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = pApplication->hInstance;
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);	//use default app icon
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);	//default cursor
	WndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);  	//nice white backgnd
	WndClass.lpszMenuName = NULL;	//default no menu
	WndClass.lpszClassName = NULL;	//class name isn't known yet

    SetDimensions(CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT);
	SetStyle(WS_OVERLAPPEDWINDOW, 0);		//default overlapped window
}	//TWindow::TWindow

TWindow::~TWindow()
{
	//can always try to unregister the window class, since the function fails without error
	//if windows still exist that are using the window class
    if (!IsFlagSet(WB_SYSTEMCLASS))
		UnregisterClass(WndClass.lpszClassName, pApplication->hInstance);
}	//TWindow::~TWindow

//done
LRESULT CALLBACK _export TWindow::StandardWndProc(HWND hWindow, UINT wMsg, WPARAM wParam, LPARAM lParam)	
{
	TMessage Msg;

	Msg.hWindow = hWindow;
	Msg.wMsg = wMsg;
	Msg.wParam = wParam;
	Msg.lParam = lParam;
	Msg.lResult = 0;

	return UMP(Msg);
}	//TWindow::StandardWndProc

//done
void TWindow::AssignMenu(int nMenuID)	
{
	AssignMenu(MAKEINTRESOURCE(nMenuID));
}	//TWindow::AssignMenu

//done
void TWindow::AssignMenu(LPCSTR lpszMenu)
{
	if (!hWindow)
		WndClass.lpszMenuName = lpszMenu;
	else
	{
        HMENU hMenu = GetMenu(hWindow);
		SetMenu(hWindow, LoadMenu(pApplication->hInstance, lpszMenu));
		DrawMenuBar(hWindow);
		if (hMenu)
			DestroyMenu(hMenu);
	}  
}	//TWindow::AssignMenu

//done
void TWindow::AssignIcon(int nIconID)  
{
    AssignIcon(MAKEINTRESOURCE(nIconID));
}	//TWindow::AssignIcon

//done
void TWindow::AssignIcon(LPCSTR lpszIcon)	
{
    HICON hNewIcon = LoadIcon(pApplication->hInstance, lpszIcon);

	if (!hWindow)
		WndClass.hIcon = hNewIcon;
	else
	{
		HICON hIcon = (HICON) GetClassWord(hWindow, GCW_HICON);
		SetClassWord(hWindow, GCW_HICON, (WORD) hNewIcon);
		DestroyIcon(hIcon);
	}
}	//TWindow::AssignIcon

//done
void TWindow::AssignCursor(int nCursorID)	
{
    AssignCursor(MAKEINTRESOURCE(nCursorID));
}	//TWindow::AssignCursor

//done
void TWindow::AssignCursor(LPCSTR lpszCursor)	
{
	HCURSOR hNewCursor = LoadCursor(pApplication->hInstance, lpszCursor);

	if (!hWindow)
		WndClass.hCursor = hNewCursor;
	else
	{
		HCURSOR hCursor = (HCURSOR) GetClassWord(hWindow, GCW_HCURSOR);
		SetClassWord(hWindow, GCW_HCURSOR, (WORD) hNewCursor);
		DestroyCursor(hCursor);
	}
}	//TWindow::AssignCursor

//done
void TWindow::WMCreate(RTMessage)
{
	//create the child windows whose WB_AUTOCREATE flags are set
	if (CreateChildren())
		//all children must exist for transfer to take place
		DoTransfer(TF_SET);
}	//TWindow::WMCreate

//done
void TWindow::WMPaint(RTMessage)   
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWindow, &ps);
	Paint(hDC, ps);
	EndPaint(hWindow, &ps);
}	//TWindow::WMPaint

//done
void TWindow::Paint(HDC, PAINTSTRUCT&)
{
	//do nothing if programmer does not supply a Paint
}	//TWindow::Paint

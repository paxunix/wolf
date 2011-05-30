/*
	TMDICHLD.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TMDIChild class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TMDIChild::TMDIChild(PTMDIFrame pParent, LPCSTR lpszTitle) : TWindow(pParent, lpszTitle, 0)
{
	ChildData.hOwner = pApplication->hInstance;
	ChildData.lParam = NULL;
	//default child style (is copied to ChildData.style when Create is called)
	dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    bCreated = FALSE;
}       //TMDIChild::TMDIChild

//done
LONG TMDIChild::DefMsgHandler(RTMessage Msg)
{
	return DefMDIChildProc(Msg.hWindow, Msg.wMsg, Msg.wParam, Msg.lParam);
}       //TMDIChild::DefMsgHandler

//done
BOOL TMDIChild::Create()
{
    Register(); //okay to register all the time
	if (!bCreated)
    {
		SetGlobalWindowObject(this);

		ChildData.szClass = WndClass.lpszClassName;
		ChildData.szTitle = lpszWindowText;
		ChildData.x = x;
		ChildData.y = y;
		ChildData.cx = cx;
		ChildData.cy = cy;
		ChildData.style = dwStyle;

		hWindow = (HWND) LOWORD(SendMessage(((PTMDIFrame) pParent)->pClient->hWindow, WM_MDICREATE, 0, (LONG) &ChildData));
		nID = GetWindowWord(hWindow, GWW_ID);   //it is set internally to ID_FIRSTMDICHILD+n
	bCreated = TRUE;
		return (BOOL) hWindow;
	}

    return FALSE;
}       //TMDIChild::Create

//done
void TMDIChild::WMMDIActivate(RTMessage Msg)
{
	if (Msg.wParam && WndClass.lpszMenuName)
	{
		HMENU hMenu = LoadMenu(pApplication->hInstance, WndClass.lpszMenuName);
		SendMessage(((PTMDIFrame) pParent)->pClient->hWindow, WM_MDISETMENU, 0, MAKELONG(hMenu, GetSubMenu(hMenu, nChildMenuPos)));
		DrawMenuBar(pParent->hWindow);
    }
}       //TMDIChild::WMMDIActivate

void TMDIChild::WMNCDestroy(RTMessage Msg)
{
	//switch frame window menu back to initial menu if it exists and this is last child wnd
	if (pParent->WndClass.lpszMenuName && !LOWORD(SendMessage(((PTMDIFrame) pParent)->pClient->hWindow, WM_MDIGETACTIVE, 0, 0)))
    {
		HMENU hMenu = LoadMenu(pApplication->hInstance, pParent->WndClass.lpszMenuName);
		SendMessage(((PTMDIFrame) pParent)->pClient->hWindow, WM_MDISETMENU, 0, MAKELONG(hMenu, GetSubMenu(hMenu, nChildMenuPos)));
		DrawMenuBar(pParent->hWindow);
	}

	TWindowObject::WMNCDestroy(Msg);
	delete this;
}       //TMDIChild::WMNCDestroy

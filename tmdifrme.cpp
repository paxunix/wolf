/*
	TMDIFRME.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TMDIFrame class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
void TMDIFrame::Init()
{
	pClient = NULL;
	WndClass.hbrBackground = (HBRUSH) COLOR_APPWORKSPACE + 1;
	nWindowMenuPos = 0;
    SetStyle(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, 0);
}	//TMDIFrame::Init

//done
TMDIFrame::TMDIFrame(LPCSTR lpszTitle, int nMenuID) : TWindow(NULL, lpszTitle, 0)
{
	AssignMenu(nMenuID);
    Init();
}	//TMDIFrame::TMDIFrame

//done
LONG TMDIFrame::DefMsgHandler(RTMessage Msg)
{
	return DefFrameProc(Msg.hWindow, pClient ? pClient->hWindow : NULL, Msg.wMsg, Msg.wParam, Msg.lParam);
}	//TMDIFrame::DefMsgHandler

//done
void TMDIFrame::CloseChildren()
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
	PTWindowObject pChild;

	while (int(ChildListIterator))
	{
		pChild = ChildListIterator++;
		SendMessage(pClient->hWindow, WM_MDIDESTROY, (WPARAM) pChild->hWindow, 0);
    };
}	//TMDIFrame::CloseChildren

//done
void TMDIFrame::WMCreate(RTMessage)
{
	pClient = new TMDIClient(this);
	pClient->Create(); 
}	//TMDIFrame::WMCreate

//done
void TMDIFrame::WMCommand(RTMessage Msg)
{
	HWND hChildWnd = (HWND) LOWORD(SendMessage(pClient->hWindow, WM_MDIGETACTIVE, 0, 0));
    PTWindowObject p;

	DispatchAMessage(Msg.LP.Lo ? Msg.wParam+ID_FIRST : Msg.wParam+CM_FIRST, Msg);
	if (hChildWnd)
	{
		p = (PTWindowObject) MAKELP(GetProp(hChildWnd, "WolfS"), GetProp(hChildWnd, "WolfO"));
		p->DispatchAMessage(Msg.LP.Lo ? Msg.wParam+ID_FIRST : Msg.wParam+CM_FIRST, Msg);
	}
    	
    DefMsgHandler(Msg);
}	//TMDIFrame::WMCommand

//done
BOOL TMDIFrame::CanClose()
{
    if (TWindowObject::CanClose())
	{
		CloseChildren();
		return TRUE;
    }
	else
		return FALSE;
}	//TMDIFrame::CanClose

/*
	TCONTROL.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TControl class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TControl::TControl(PTWindowObject pParent, int nID) : TWindow(pParent, NULL, nID)
{
	if (pParent->Identify() != dialogClass && pParent->Identify() != modelessdialogClass)
		SetFlags(WB_AUTOCREATE, TRUE);

    SetFlags(WB_SYSTEMCLASS, TRUE);
	lpfnOldWndProc = NULL;
}	//TControl::TControl

//done
BOOL TControl::Create()
{
	BOOL bRetval;

	dwStyle |= WS_CHILD;
	bRetval = TWindowObject::Create();

	if (bRetval)
        SetProperties();

    return bRetval;
}	//TControl::Create

//done
void TControl::SetRedraw(BOOL bRedraw)
{
	SendMessage(hWindow, WM_SETREDRAW, (WPARAM) bRedraw, 0);
}	//TControl::SetRedraw

//done
void TControl::SubClass()
{
	lpfnOldWndProc = (WNDPROC) SetWindowLong(hWindow, GWL_WNDPROC, (LONG) TWindow::StandardWndProc);
	SetFlags(WB_SYSTEMCLASS, FALSE);	//don't delete its properties when parent is killed
										//see TWindowObject::WMDestroy
}	//TControl::SubClass

//done
void TControl::UnSubClass()
{
	SetWindowLong(hWindow, GWL_WNDPROC, (LONG) lpfnOldWndProc);
	lpfnOldWndProc = NULL;
    SetFlags(WB_SYSTEMCLASS, TRUE);		//delete its properties when parent is killed
}	//TControl::UnSubClass

//done
LONG TControl::DefMsgHandler(RTMessage Msg)
{
    if (lpfnOldWndProc)	//control has called SubClass(), so call old window procedure
		return CallWindowProc(lpfnOldWndProc, Msg.hWindow, Msg.wMsg, Msg.wParam, Msg.lParam);
	else
		return TWindow::DefMsgHandler(Msg);	//control has own window class
}	//TControl::DefMsgHandler

//done
void TControl::WMPaint(RTMessage Msg)
{
	//let DefMsgHandler take care of it.  If you want your controls to have their own
	//paint procedures, overload WMPaint to call TWindow::Paint and supply your own
	//Paint() procedure as for normal windows.
	DefMsgHandler(Msg);
}	//TControl::WMPaint

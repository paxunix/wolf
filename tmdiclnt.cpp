/*
	TMDICLNT.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TMDIClient class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#ifndef __SHDDEL_H
#include <shddel.h>
#endif

//done
TMDIClient::TMDIClient(PTMDIFrame pParent) : TWindow(pParent, NULL, 0)
{
	ClientData.idFirstChild = ID_FIRSTMDICHILD;
	WndClass.lpszClassName = "MDICLIENT";
	SetStyle(MDIS_ALLCHILDSTYLES | WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, 0);
	pFrame = pParent;
	x = y = cx = cy = 0;
	SetFlags(WB_TRANSFER, FALSE);

	//remove client object from parent's child list
	pParent->ChildList.detach(this, TShouldDelete::Delete);
}	//TMDIClient::TMDIClient

//done
BOOL TMDIClient::Create()
{
	//no need to set up dynamic dispatcher (call SetGlobalWindowObject) because this is
	//a system window class.
	//create the window
	ClientData.hWindowMenu = GetSubMenu(GetMenu(pFrame->hWindow), pFrame->nWindowMenuPos);
	hWindow = CreateWindowEx(dwExStyle, WndClass.lpszClassName, "", dwStyle,
	 x, y, cx, cy,
	 pFrame->hWindow, 0, pApplication->hInstance, (LPSTR) &ClientData);
	return hWindow != NULL;
}	//TMDIClient::Create

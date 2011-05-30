/*
	TPUSHBTN.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TPushButton class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TPushButton::TPushButton(PTWindowObject pParent, int nID) : TControl(pParent, nID)
{
	SetStyle(WS_VISIBLE | BS_PUSHBUTTON, 0);
	WndClass.lpszClassName = "BUTTON";
}	//TPushButton::TPushButton

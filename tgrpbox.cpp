/*
	TGRPBOX.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TGroupBox class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

TGroupBox::TGroupBox(PTWindowObject pParent, int nID) : TPushButton(pParent, nID)
{
	SetStyle(WS_VISIBLE | BS_GROUPBOX, 0);
}	//TGroupBox::TGroupBox

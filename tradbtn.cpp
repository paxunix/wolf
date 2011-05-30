/*
	TRADBTN.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TRadioButton class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TRadioButton::TRadioButton(PTWindowObject pParent, int nID, int nStartID, int nEndID) : TCheckBox(pParent, nID)
{
	this->nStartID = nStartID;
	this->nEndID = nEndID;
	SetStyle(WS_VISIBLE | BS_AUTORADIOBUTTON, 0);
}	//TRadioButton::TRadioButton

//done
int TRadioButton::GetCheck()
{
	int i;

	for (i=nStartID; i<=nEndID; i++)
		if (IsDlgButtonChecked(pParent->hWindow, i))
			return i;

	return 0;
}	//TRadioButton::GetCheck

//done
void TRadioButton::SetCheck(int nID)
{
	CheckRadioButton(pParent->hWindow, nStartID, nEndID, nID);
}	//TRadioButton::SetCheck

/*
	TCHKBOX.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TCheckBox class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TCheckBox::TCheckBox(PTWindowObject pParent, int nID) : TPushButton(pParent, nID)
{
	SetStyle(WS_VISIBLE | BS_AUTOCHECKBOX, 0);
}	//TCheckBox::TCheckBox

//done
WORD TCheckBox::Transfer(LPSTR lpCur, TransDir Dir)
{
	LPWORD lpData = (LPWORD) lpCur;

	switch (Dir)
	{
		case TF_SET:
			SetCheck(*lpData);
           	break;
	
		case TF_GET:
			*lpData = GetCheck();
	}

	return checkboxTFSize;
}	//TCheckBox::Transfer

//done
int TCheckBox::GetCheck()	
{
	return (int) IsDlgButtonChecked(pParent->hWindow, nID);
}	//TCheckBox::GetCheck

//done
void TCheckBox::SetCheck(int nStatus)
{
	CheckDlgButton(pParent->hWindow, nID, nStatus);
}	//TCheckBox::SetCheck

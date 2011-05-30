/*
	TCOMBOX.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TComboBox class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TComboBox::TComboBox(PTWindowObject pParent, int nID) : TListBox(pParent, nID)
{
	SetStyle(WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS, 0);
	bIsListBox = FALSE;
	WndClass.lpszClassName = "COMBOBOX";
}	//TComboBox::TComboBox

//done
int TComboBox::GetEditSel(int &nStart, int &nEnd)
{
	LRESULT lResult = SendMessage(hWindow, CB_GETEDITSEL, 0, 0);
	nStart = LOWORD(lResult);
	nEnd = HIWORD(lResult);
    return (int) lResult;
}	//TComboBox::GetEditSel

//done
int TComboBox::ShowList(BOOL bShow)
{
	return (int) SendMessage(hWindow, CB_SHOWDROPDOWN, (WPARAM) bShow, 0);
}	//TComboBox::ShowList

//done
int TComboBox::SetEditSel(int nStart, int nEnd)
{
	return (int) SendMessage(hWindow, CB_SETEDITSEL, 0, MAKELPARAM(nStart, nEnd));
}	//TComboBox::SetEditSel

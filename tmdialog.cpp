/*
	TMDIALOG.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TModelessDialog class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TModelessDialog::TModelessDialog(PTWindowObject pParent, int nID) : TDialog(pParent, nID)
{
	lpfnDlgProc = NULL;
	WndClass.cbWndExtra = DLGWINDOWEXTRA;
	SetFlags(WB_SYSTEMCLASS | WB_KBHANDLER, TRUE);
}	//TModelessDialog::TModelessDialog

//done
BOOL TModelessDialog::Create()
{
    Register();
	if (!hWindow)
	{
		SetGlobalWindowObject(this);
		hWindow = CreateDialog(pApplication->hInstance, MAKEINTRESOURCE(nID), pParent->hWindow, TDialog::DlgProc);
		return hWindow != NULL;
	}

	return FALSE;
}	//TModelessDialog::Create

//done
void TModelessDialog::Destroy(int nStatus)
{
    if (CanClose())
	{
		DestroyWindow(hWindow);
		this->nStatus = nStatus;
	}
}	//TModelessDialog::Destroy

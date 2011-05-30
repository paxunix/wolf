/*
	TINPDLG.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TInputDialog class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

extern HINSTANCE hLibInst;

//done
TInputDialog::TInputDialog(PTWindowObject pParent, LPCSTR lpszQuery, LPSTR lpszBuffer, int nBufLen) : TDialog(pParent, ID_INPUTDLG)
{
	this->lpszQuery = lpszQuery;
	this->lpszBuffer = lpszBuffer;
	this->nBufLen = nBufLen;
	new TEdit(this, 101, nBufLen);
	SetTransferBuffer(lpszBuffer);
}	//TInputDialog::TInputDialog

//done
BOOL TInputDialog::Create()
{
	SetGlobalWindowObject(this);
	return DialogBox(hLibInst, MAKEINTRESOURCE(nID), pParent->hWindow, TDialog::DlgProc) != -1;
}	//TInputDialog::Create

//done
LRESULT TInputDialog::InitDialog()
{
    SetDlgItemText(hWindow, 100, lpszQuery);
    GetChild(101)->SetFocus();
    return 0;
}	//TInputDialog::InitDialog

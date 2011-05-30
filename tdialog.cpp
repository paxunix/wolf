/*
	TDIALOG.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TDialog class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TDialog::TDialog(PTWindowObject pParent, int nID) : TWindow(pParent, NULL, nID)
{
	nStatus = 0;
}	//TDialog::TDialog

//done
BOOL CALLBACK _export TDialog::DlgProc(HWND hWindow, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	TMessage Msg;

	Msg.hWindow = hWindow;
	Msg.wMsg = wMsg;
	Msg.wParam = wParam;
	Msg.lParam = lParam;
	Msg.lResult = TRUE;

	return (BOOL) UMP(Msg);
}	//TDialog::DlgProc

//done
BOOL TDialog::Create()
{
	SetGlobalWindowObject(this);
	return DialogBox(pApplication->hInstance, MAKEINTRESOURCE(nID), pParent->hWindow, TDialog::DlgProc) != -1;
}	//TDialog::Create

//done
LONG TDialog::DefMsgHandler(RTMessage)
{
	return FALSE;
}	//TDialog::DefMsgHandler

//done
void TDialog::WMInitDialog(RTMessage Msg)
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
	PTWindowObject pChild;

	while (int(ChildListIterator))
	{
		pChild = ChildListIterator++;
        pChild->hWindow = GetDlgItem(hWindow, pChild->nID); 
		pChild->SetProperties();
	};

	DoTransfer(TF_SET);
	Msg.lResult = InitDialog();
}	//TDialog::WMInitDialog

//done
void TDialog::Okay(RTMessage)
{
	//make sure dialog's transfer mechanism is active, since user clicked OK
	//Transfer is performed in TWindowObject::WMDestroy
    SetFlags(WB_TRANSFER, TRUE);
	Destroy(IDOK);
}	//TDialog::Okay

//done
void TDialog::Cancel(RTMessage)
{
	//turn off transfer mechanism for dialog so it doesn't work when DoTransfer is
	//called in TWindowObject::WMDestroy
    SetFlags(WB_TRANSFER, FALSE);
	Destroy(IDCANCEL);
}	//TDialog::Cancel

//done
void TDialog::Destroy(int nStatus)
{
	if (CanClose())
    {
		this->nStatus = nStatus;
		EndDialog(hWindow, TRUE);
	}
}	//TDialog::Destroy

//done
void TDialog::WMClose(RTMessage Msg)
{
	Cancel(Msg);
}	//TDialog::WMClose

//done
WORD TDialog::GetDefault()
{
	return (LOWORD(SendMessage(hWindow, DM_GETDEFID, 0, 0)));
}	//TDialog::GetDefault

//done
void TDialog::SetDefault(int nID)
{
	SendMessage(hWindow, DM_SETDEFID, nID, 0);
}	//TDialog::SetDefault

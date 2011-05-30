/*
	TWINDOBJ.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TWindowObject class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#ifndef __SHDDEL_H
#include <shddel.h>
#endif

#ifndef __MEM_H
#include <mem.h>
#endif

extern PTApplication pTempApp;

//done
TWindowObject::TWindowObject(PTWindowObject pAParent, LPCSTR lpszAWindowText, int nAnID)
{
	if (pAParent)
	{
		pAParent->ChildList.addAtTail(this);
		pApplication = pAParent->pApplication;
    }
    else
	{
		pApplication = pTempApp;	//get it where we put it when in TApp constructor
		pTempApp = NULL;			//and set it to 0 to prevent surprises
	}
	    	
	lpTransferBuffer = NULL;	//no transfer buffer assigned
	dwFlags = 0;	//don't auto create; no system class; no kbd handler
	SetFlags(WB_TRANSFER, (pAParent != NULL));	//enable transfer mechanism unless main window

	pParent = pAParent;	//save parent
	hWindow = 0;	//will be assigned when window is created
	x = y = cx = cy = 0;	//to 0
	dwStyle = dwExStyle = 0;
	lpszWindowText = lpszAWindowText;
	nID = nAnID;	//save window's ID
	_fmemset(&WndClass, 0, sizeof(WNDCLASS));
}	//TWindowObject::TWindowObject

//done
TWindowObject::~TWindowObject()
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);

	//delete all children
	while (int(ChildListIterator))
        delete ChildListIterator++;

	//remove this from parent's list
	if (pParent)
		pParent->ChildList.detach(this, TShouldDelete::Delete);
}	//TWindowObject::~TWindowObject

//done
void TWindowObject::SetProperties()	
{
	SetProp(hWindow, "WolfS", (HANDLE) SELECTOROF(this));
	SetProp(hWindow, "WolfO", (HANDLE) OFFSETOF(this));
}	//TWindowObject::SetProperties

//done
void TWindowObject::RemoveProperties()	
{
	RemoveProp(hWindow, "WolfS");
    RemoveProp(hWindow, "WolfO");
}	//TWindowObject::RemoveProperties

//done
BOOL TWindowObject::IsFlagSet(DWORD dwMask)	
{
	return (dwFlags & dwMask) != 0;
}	//TWindowObject::IsFlagSet

//done
void TWindowObject::SetFlags(DWORD dwMask, BOOL bOn)	
{
	if (bOn)
		dwFlags |= dwMask;
	else
    	dwFlags &= ~dwMask;
}	//TWindowObject::SetFlags

//done
void TWindowObject::SetTransferBuffer(LPSTR lpBuffer)
{
	lpTransferBuffer = lpBuffer;
}	//TWindowObject::SetTransferBuffer

//done
BOOL TWindowObject::CreateChildren()
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
	PTWindowObject pChild;
	BOOL bRetval = TRUE;

    //loop through this object's child window list
	while (int(ChildListIterator))
    {
        pChild = ChildListIterator++;
		if (pChild->IsFlagSet(WB_AUTOCREATE))	//if child's WB_AUTOCREATE flag is set
			if (!pChild->Create())				//try to create child now.
				bRetval = FALSE;				//return FALSE if one creation fails
    };

	return bRetval;		//return TRUE if all children successfully created
}	//TWindowObject::CreateChildren

//done
void TWindowObject::WMCommand(RTMessage Msg)	
{
	//now dispatch message to the MRF responsible for the CM, ID, etc.
	DispatchAMessage(Msg.LP.Lo ? Msg.wParam+ID_FIRST : Msg.wParam+CM_FIRST, Msg);
}	//TWindowObject::WMCommand

//done
void TWindowObject::WMClose(RTMessage Msg)
{
	if (CanClose())		//see if okay to close window now
		DefMsgHandler(Msg);		//go ahead
}	//TWindowObject::WMClose

//done
void TWindowObject::WMDestroy(RTMessage Msg)
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
    PTWindowObject pChild;

	DoTransfer(TF_GET);		//window being destroyed.  Get data from children.
	while (int(ChildListIterator))		//loop through child windows
	{
		pChild = ChildListIterator++;
		//only remove properties of children who have a predefined window class and are not
		//subclassed, otherwise it will delete its own properties.
		if (pChild->IsFlagSet(WB_SYSTEMCLASS))
        	pChild->RemoveProperties();
	};

	if (!pParent)		// is this the last window of the app
		PostQuitMessage(0);		//we're done
	else
		DefMsgHandler(Msg);		
}	//TWindowObject::WMDestroy

//done
void TWindowObject::WMNCDestroy(RTMessage)	
{
	//since the WM_NCDESTROY message is the last possible message a window will
	//receive it is the place that the properties used by WOLF must be removed.
    RemoveProperties();
	hWindow = 0;
}	//TWindowObject::WMNCDestroy

//done
void TWindowObject::WMHScroll(RTMessage Msg)
{
	PTScrollBar pScrollBar;

    //dispatch to appropriate scroll bar if msg is for a scrollbar control 
	if (Msg.LP.Hi)
	{
		pScrollBar = (PTScrollBar) MAKELP(GetProp((HWND) Msg.LP.Hi, "WolfS"), GetProp((HWND) Msg.LP.Hi, "WolfO"));
		pScrollBar->DoScroll(Msg);
	}
	else
		DefMsgHandler(Msg);
}	//TWindowObject::WMHScroll

//done
void TWindowObject::WMVScroll(RTMessage Msg)
{
	WMHScroll(Msg);		//looks funny but it works since virtual functions do all the work
}	//TWindowObject::WMHScroll

//done
void TWindowObject::WMQueryEndSession(RTMessage Msg)
{
	Msg.lResult = CanClose();
}	//TWindowObject::WMQueryEndSession

//done
BOOL TWindowObject::Create()
{
	Register();		//can register all the time
	SetGlobalWindowObject(this);	//set up dynamic dispatching data

	//create the window
	hWindow = CreateWindowEx(dwExStyle, WndClass.lpszClassName, lpszWindowText, dwStyle,
	 x, y, cx, cy,
	 pParent ? pParent->hWindow : 0, (HMENU) nID, pApplication->hInstance, NULL);
	return (BOOL) hWindow;
}	//TWindowObject::Create

//done
BOOL TWindowObject::Register()
{
	//only register WndClass if this window class is not predefined and this is the first
	//instance of the app
	if (IsFlagSet(WB_SYSTEMCLASS) || pApplication->hPrevInstance)
		return TRUE;
	else
		return RegisterClass(&WndClass);
}	//TWindowObject::Register

//done
PTWindowObject TWindowObject::GetChild(int nID)	//done
{
	PTWindowObject pObj = (PTWindowObject) MAKELP(GetProp(GetDlgItem(hWindow, nID), "WolfS"), GetProp(GetDlgItem(hWindow, nID), "WolfO"));
	if (pObj)
		return pObj;
	else
	{
		BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
	    PTWindowObject pChild = NULL;

		while (int(ChildListIterator))
		{
			pChild = ChildListIterator++;
			if (pChild->nID == nID)
                break;
        };

		return pChild;
    }
}	//TWindowObject::GetChild

//done
void TWindowObject::DoTransfer(TransDir Dir)
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
    PTWindowObject pChild;
	LPSTR lpCur = lpTransferBuffer;

	//if this object's transfer flag is on and has a valid transfer buffer,
	//try to transfer to all children
	if (IsFlagSet(WB_TRANSFER) && lpTransferBuffer)
		while (int(ChildListIterator))
		{
			pChild = ChildListIterator++;
			//child must have transfer enabled too
			if (pChild->IsFlagSet(WB_TRANSFER))
				lpCur += pChild->Transfer(lpCur, Dir);	//transfer the data from/to child
			else
				//skip the data in the transfer buffer that should go to the child
            	lpCur += pChild->Transfer(lpCur, TF_SIZE);
		};
}	//TWindowObject::DoTransfer

//done
BOOL TWindowObject::CanClose()
{
	BI_DoubleListIteratorImp<PTWindowObject> ChildListIterator(ChildList);
	PTWindowObject pChild;

    //loop through all children
	while (int(ChildListIterator))
	{
		pChild = ChildListIterator++;
		if (!pChild->CanClose())	//if one child says it can't close return FALSE
        	return FALSE;
	};

	return TRUE;
}	//TWindowObject::CanClose

//done
LONG TWindowObject::DispatchAMessage(UINT wMRFIndex, RTMessage Msg)
{

	LPDWORD pVT = (LPDWORD)(LPSTR) this;
    LPDWORD pfn;
	pVT = (LPDWORD) *pVT;

	//this function is not prototyped in any of BORLAND's header files, but it
    //is public and is supposed to be used for exactly what I'm using it for
	MRFUNCTION fMRF = _DDVTdispatchNULL((LPVOID) pVT, wMRFIndex);
	pfn = (LPDWORD)(LPSTR) fMRF;
	fMRF = (MRFUNCTION) *pfn;

	if (fMRF)
	{
		(*fMRF)(this, Msg);
		return Msg.lResult;
    }
    else
		return DefMsgHandler(Msg);
}	//TWindowObject::DispatchAMessage

//done
LONG TWindowObject::DefMsgHandler(RTMessage Msg)
{
	return DefWindowProc(hWindow, Msg.wMsg, Msg.wParam, Msg.lParam);
}	//TWindowObject::DefMsgHandler

//done
void TWindowObject::Show(int nHow)	
{
	ShowWindow(hWindow, nHow);
}	//TWindowObject::Show

//done
void TWindowObject::SetText(LPCSTR lpszText)	
{
	lpszWindowText = lpszText;

	if (hWindow)
		SetWindowText(hWindow, lpszText);
}	//TWindowObject::SetText

//done
void TWindowObject::GetText(LPSTR lpszDest, int cbDestLen)
{
	GetWindowText(hWindow, lpszDest, cbDestLen);
}	//TWindowObject::GetText

//done
int TWindowObject::GetTextLen()	
{
	return GetWindowTextLength(hWindow);
}	// TWindowObject::GetTextLen

//done
void TWindowObject::Enable(BOOL bEnable)	
{
	EnableWindow(hWindow, bEnable);
}	//TWindowObject::Enable

//done
void TWindowObject::SetDimensions(int left, int right, int width, int height)	
{
	x = left;
	y = right;
	cx = width;
	cy = height;
}	//TWindowObject::SetDimensions

//done
void TWindowObject::GetDimensions()	
{
	RECT r;

	GetWindowRect(hWindow, &r);
	if (pParent)
	{
		ScreenToClient(pParent->hWindow, (LPPOINT) &r.left);
		ScreenToClient(pParent->hWindow, (LPPOINT) &r.right);
    }

    x = r.left;
	y = r.top;
	cx = r.right - x;
	cy = r.bottom - y;
}	//TWindowObject::GetDimensions

//done
void TWindowObject::SetStyle(DWORD dwS, DWORD dwExS)	
{
	dwStyle = dwS;
	dwExStyle = dwExS;

	if (hWindow)
	{
		SetWindowLong(hWindow, GWL_STYLE, dwStyle);
		SetWindowLong(hWindow, GWL_EXSTYLE, dwExStyle);
	}
}	//TWindowObject::SetStyle

//done
void TWindowObject::GetStyle()	
{
	dwStyle = GetWindowLong(hWindow, GWL_STYLE);
	dwExStyle = GetWindowLong(hWindow, GWL_EXSTYLE);
}	//TWindowObject::GetStyle

//done
void TWindowObject::SetFocus()
{
	::SetFocus(hWindow);
}	//TWindowObect::SetFocus

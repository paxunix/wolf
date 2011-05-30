/*
	TTOOLBAR.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for WOLF's TToolBar class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#ifndef __MEM_H
#include <mem.h>
#endif

#ifndef __STDLIB_H
#include <stdlib.h>
#endif

extern HINSTANCE hLibInst;

HBITMAP TToolBar::DuplicateBitmap(HBITMAP hbmpSrc)
{
    HBITMAP hbmpNew;
    HDC     hdcSrc, hdcDest;

    hdcSrc = CreateCompatibleDC(NULL);
    hdcDest = CreateCompatibleDC(hdcSrc);

	SelectObject(hdcSrc, hbmpSrc);
    hbmpNew = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);
	SelectObject(hdcDest, hbmpNew);

	BitBlt(hdcDest, 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
	DeleteDC(hdcDest);
    DeleteDC(hdcSrc);
	return hbmpNew;
}	//TToolBar::DuplicateBitmap

void TToolBar::DrawBitmap(HDC hdc, HBITMAP hbm, DWORD dwRop)
{
	HDC hdcMem;

	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hbm);
	BitBlt(hdc, 0, 0, nWidth, nHeight, hdcMem, 0, 0, dwRop);
    DeleteDC(hdcMem);
}	//TToolBar::DrawBitmap

TToolBar::TToolBar(PTWindowObject pParent, LPCSTR lpszWindowTitle, PTToolBarInfo pBInfo, int nNumBtns) :
 TWindow(pParent, lpszWindowTitle)
{
	int i, i2;
	BITMAP bmp;

	nNumButtons = nNumBtns;
	nMaxWidth = GetSystemMetrics(SM_CXSCREEN) / 2;
	pInfo = new TToolBarInfo[nNumButtons];
	_fmemcpy(pInfo, pBInfo, sizeof(TToolBarInfo) * nNumButtons);

	dwStyle = WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_VISIBLE | WS_SYSMENU;
	SetFlags(WB_AUTOCREATE | WB_KBHANDLER, TRUE);
	SetFlags(WB_TRANSFER | WB_SYSTEMCLASS, FALSE);
	WndClass.hbrBackground = (HBRUSH) COLOR_BTNFACE + 1;
	WndClass.lpszClassName = "WOLF::TTOOLBAR";
    WndClass.style |= CS_NOCLOSE;	//can't use close option

	hPatBmp = LoadBitmap(hLibInst, "GRAYINGBMP");
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	for (i=0; i<nNumButtons; i++)
		if ((pInfo+i)->nID)	//if not a space
        {
            i2 = i;	//keep track of one non-space bmp so we can get its size
            //load all the bitmaps for button and store the handles
			if (!(pInfo+i)->hBmp[0])	//normal, no focus
				(pInfo+i)->hBmp[0] = LoadBitmap(pApplication->hInstance, MAKEINTRESOURCE((pInfo+i)->nID + 1000));

			//although this bmp is only for 3-state, LoadBitmap returns 0 if it can't find it,
            //so we can load nonexistent bmps with no problems
            if (!(pInfo+i)->hBmp[1])	//pressed, no focus
				(pInfo+i)->hBmp[1] = LoadBitmap(pApplication->hInstance, MAKEINTRESOURCE((pInfo+i)->nID + 2000));

			if (!(pInfo+i)->hBmp[2])	//pressed, focus
				(pInfo+i)->hBmp[2] = LoadBitmap(pApplication->hInstance, MAKEINTRESOURCE((pInfo+i)->nID + 3000));

			if (!(pInfo+i)->hBmp[3])	//normal, focus
				(pInfo+i)->hBmp[3] = LoadBitmap(pApplication->hInstance, MAKEINTRESOURCE((pInfo+i)->nID + 5000));

			ConstructButton((pInfo+i)->nID);
		}

	GetObject((pInfo+i2)->hBmp[0], sizeof(BITMAP), &bmp);
	nWidth = bmp.bmWidth;
	nHeight = bmp.bmHeight;

	cx = cy = 0;	//Tool bar window size will be determined in WM_SIZE
	bDone = FALSE;	//used in WM_SIZE to prevent infinite recursion

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}	//TToolBar::TToolBar

TToolBar::~TToolBar()
{
	int i, j;

	for (i=0; i<nNumButtons; i++)
		for (j=0; j<4; j++)
			if ((pInfo+i)->hBmp[j])	//valid bmp handle?
				DeleteObject((pInfo+i)->hBmp[j]);

    DeleteObject(hPatBmp);
	delete[] pInfo;
}	//TToolBar::~TToolBar

void TToolBar::WMSize(RTMessage Msg)
{
	int nNewWndWidth, nx, ny, i, nNewCX, nNewCY;
    PTPushButton pb;

	if (bDone)	//prevent infinite recursion for this object
	{
		bDone = FALSE;
		return;
	}

	nNewWndWidth = Msg.LP.Lo / nWidth * nWidth;

	//reposition the buttons.  They must be positioned in the order they exist in pInfo,
    //which is not necessarily the order in ChildList (if any removals/additions have occurred)
    nx = ny = 0;
	for (i=0; i<nNumButtons; i++)
	{
		if (nx >= nNewWndWidth)
		{
			nx = 0;
			ny += nHeight;
        }

		if ((pInfo+i)->nID)	//isn't a space
		{
			pb = (PTPushButton) GetChild((pInfo+i)->nID);
			SetWindowPos(pb->hWindow, 0, nx, ny, nWidth, nHeight, SWP_NOACTIVATE | SWP_NOZORDER);
		}

		nx += nWidth;
	}

    bDone = TRUE;
    CalcWindowSize(nNewWndWidth, ny + nHeight, nNewCX, nNewCY);
	SetWindowPos(hWindow, 0, 0, 0, nNewCX, nNewCY, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}	//TToolBar::WMSize
    	
void TToolBar::CalcWindowSize(int nNewClientWidth, int nNewClientHeight, int& nNewCX, int& nNewCY)
{
	nNewCX = nNewClientWidth + GetSystemMetrics(SM_CXFRAME) * 2;
	nNewCY = nNewClientHeight + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 - 1;
}	//TToolBar::CalcWindowSize

PTPushButton TToolBar::ConstructButton(int nID)
{
	PTPushButton pb;

	pb = new TPushButton(this, nID);
	pb->SetStyle(BS_OWNERDRAW | WS_VISIBLE | WS_TABSTOP, 0);
	pb->SetDimensions(0, 0, 0, 0);

	return pb;
}	//TToolBar::ConstructButton

void TToolBar::WMDrawItem(RTMessage Msg)
{
    LPDRAWITEMSTRUCT lpdis = (LPDRAWITEMSTRUCT) Msg.lParam;
	int i, nBmpOffset;

	//determine the TToolBarInfo that goes with the button's ID
	for (i=0; i<nNumButtons && (pInfo+i)->nID != lpdis->CtlID; i++);

	if ((pInfo+i)->nType == NORMAL)
		switch (lpdis->itemAction)
		{
			case ODA_DRAWENTIRE:
				if ((lpdis->itemState & ODS_DISABLED) != 0)
				{
					HBRUSH hbr = CreatePatternBrush(hPatBmp);
					HDC hdcMem = CreateCompatibleDC(lpdis->hDC);
					//duplicate bmp because we don't want to modify original when PatBlt'ing
					HBITMAP hNewBmp = DuplicateBitmap((pInfo+i)->hBmp[0]);

					SelectObject(hdcMem, hNewBmp);
					BitBlt(lpdis->hDC, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);	//prepare dest bmp

					SelectObject(hdcMem, hbr);
					PatBlt(hdcMem, 0, 0, nWidth, nHeight, 0xFA0089L);	//gray the bmp
					BitBlt(lpdis->hDC, 0, 0, nWidth, nHeight, hdcMem, 0, 0, 0x900C45L); //combine with dest bmp

					DeleteDC(hdcMem);
					DeleteObject(hbr);
                    DeleteObject(hNewBmp);
					return;		//we're using a different ROP code so don't redraw bitmap
				}
				else
					nBmpOffset = 0;
				break;

			case ODA_SELECT:
				if ((lpdis->itemState & ODS_SELECTED) != 0)	//push button in
					nBmpOffset = 2;
				else	//button pops out
					nBmpOffset = 3; 
				break;

			case ODA_FOCUS:
				if ((lpdis->itemState & ODS_FOCUS) == 0)
					nBmpOffset = 0;
				else
					nBmpOffset = 3;
		}
	else
	if ((pInfo+i)->nType == THREESTATE)
		switch (lpdis->itemAction)
		{
			case ODA_DRAWENTIRE:
				if ((lpdis->itemState & ODS_DISABLED) != 0)
				{
					HBRUSH hbr = CreatePatternBrush(hPatBmp);
					HDC hdcMem = CreateCompatibleDC(lpdis->hDC);
					//duplicate bmp because we don't want to modify original when PatBlt'ing
					HBITMAP hNewBmp = DuplicateBitmap((pInfo+i)->hBmp[0]);

					SelectObject(hdcMem, hNewBmp);
					BitBlt(lpdis->hDC, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY);	//prepare dest bmp

					SelectObject(hdcMem, hbr);
				    PatBlt(hdcMem, 0, 0, nWidth, nHeight, 0xFA0089L);	//gray the bmp
					BitBlt(lpdis->hDC, 0, 0, nWidth, nHeight, hdcMem, 0, 0, 0x900C45L); //combine with dest bmp

					DeleteDC(hdcMem);
					DeleteObject(hbr);
                    DeleteObject(hNewBmp);
					return;		//we're using a different ROP code so don't redraw bitmap
				}
				else
					nBmpOffset = (pInfo+i)->bDown ? 1 : 0;
				break;

			case ODA_SELECT:
				if ((lpdis->itemState & ODS_SELECTED) != 0)
					(pInfo+i)->bDown = !(pInfo+i)->bReserved;
				else
				{
					DWORD dw = GetMessagePos();
                    POINT pt = MAKEPOINT(dw);

					ScreenToClient(lpdis->hwndItem, &pt);
					#pragma warn -wstv	//turn of Structure passed by value warning
					if (!PtInRect(&(lpdis->rcItem), pt))
						(pInfo+i)->bDown = (pInfo+i)->bReserved;
                    #pragma warn .wstv
                }

				nBmpOffset = (pInfo+i)->bDown ? 2 : 3;
				break;

			case ODA_FOCUS:
				(pInfo+i)->bReserved = (pInfo+i)->bDown;
				if ((lpdis->itemState & ODS_FOCUS) == 0)
					nBmpOffset = (pInfo+i)->bDown ? 1 : 0;
				else
					nBmpOffset = (pInfo+i)->bDown ? 2 : 3;
		};

	DrawBitmap(lpdis->hDC, (pInfo+i)->hBmp[nBmpOffset], SRCCOPY);
}	//TToolBar::WMDrawItem

void TToolBar::WMCommand(RTMessage Msg)
{
	int i;

	TWindow::WMCommand(Msg);

	if (Msg.LP.Lo)	//msg is from a control (assumably a button)
	{
        //Determine TToolBarInfo that goes with the button
		for (i=0; i<nNumButtons && (pInfo+i)->nID != Msg.wParam; i++);

		(pInfo+i)->bReserved = (pInfo+i)->bDown;
	}
}	//TToolBar::WMCommand

void TToolBar::WMGetMinMaxInfo(RTMessage Msg)
{
	MINMAXINFO FAR *lpMinMax = (MINMAXINFO FAR *) Msg.lParam;	//always FAR ptrs

	lpMinMax->ptMinTrackSize.x = max(nWidth + GetSystemMetrics(SM_CXFRAME) * 2, GetSystemMetrics(SM_CXMINTRACK));
	lpMinMax->ptMaxTrackSize.x = nMaxWidth;
}	//TToolBar::WMGetMinMaxInfo

void TToolBar::AddButton(PTToolBarInfo pNewBtnInfo, int nNewBtnPos)
{
	PTToolBarInfo pNewBarInfo, pCur;
    PTPushButton pb;

	nNumButtons++;
	pCur = pNewBarInfo = new TToolBarInfo[nNumButtons];

    //copy info before new button position
	_fmemcpy(pCur, pInfo, sizeof(TToolBarInfo) * nNewBtnPos);
	pCur += nNewBtnPos;

	//copy new button info
	_fmemcpy(pCur, pNewBtnInfo, sizeof(TToolBarInfo));
	pCur++;

	//copy rest of button info
	_fmemcpy(pCur, pInfo+nNewBtnPos, sizeof(TToolBarInfo) * (nNumButtons - 1 - nNewBtnPos));

	delete[] pInfo;		//kill old info
	pInfo = pNewBarInfo;

	//make WOLF object and interface object for new button
	pb = new TPushButton(this, (pInfo+nNewBtnPos)->nID);
	pb->SetStyle(BS_OWNERDRAW | WS_VISIBLE | WS_TABSTOP, 0);
	pb->SetDimensions(0, 0, 0, 0);
    pb->Create();

	//force resize and redraw of toolbar
	SetWindowPos(hWindow, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}	//TToolBar::AddButton

void TToolBar::RemoveButton(int nPos)
{
	PTToolBarInfo pNewBarInfo, pCur;
    PTPushButton pb = (PTPushButton) GetChild((pInfo+nPos)->nID);

	for (int i=0; i<4; i++)
		if ((pInfo+nPos)->hBmp[i])	//valid bmp handle?
			DeleteObject((pInfo+nPos)->hBmp[i]);

	nNumButtons--;
	pCur = pNewBarInfo = new TToolBarInfo[nNumButtons];

    //copy info before button position
	_fmemcpy(pCur, pInfo, sizeof(TToolBarInfo) * nPos);
	pCur += nPos;

	//copy rest of button info after button we're deleting
	_fmemcpy(pCur, pInfo+nPos+1, sizeof(TToolBarInfo) * (nNumButtons - nPos));

	delete[] pInfo;		//kill old info
	pInfo = pNewBarInfo;

	//destroy the interface object
	SendMessage(pb->hWindow, WM_CLOSE, 0, 0);
	//destroy the WOLF object
    delete pb;
	//force resize and redraw of toolbar
	SetWindowPos(hWindow, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
}	//TToolBar::RemoveButton

void TToolBar::SetState(int nID, BOOL bDown)
{
	PTToolBarInfo pCur = pInfo + GetPos(nID);

	pCur->bDown = bDown;
	if (hWindow)
	{
		InvalidateRect(GetChild(nID)->hWindow, NULL, FALSE);
		UpdateWindow(GetChild(nID)->hWindow);
    }
}	//TToolBar::SetState

int TToolBar::GetPos(int nID)
{
	for (int i=0; i<nNumButtons; i++)
		if ((pInfo+i)->nID == nID)
			return i;

	return 32767;	//something large enough to probably cause a GP fault,
    //which is fine, because if the ID isn't found, we show an error
}	//TToolBar::GetPos

void TToolBar::SetType(int nID, ToolBarBtnType Type)
{
	PTToolBarInfo pCur = pInfo + GetPos(nID);

    pCur->nType = Type;
	if (hWindow)
	{
		InvalidateRect(GetChild(nID)->hWindow, NULL, FALSE);
		UpdateWindow(GetChild(nID)->hWindow);
    }
}	//TToolBar::SetType

void TToolBar::SetBitmap(int nID, int nWhichHandle, HBITMAP hBmp)
{
	PTToolBarInfo pCur = pInfo + GetPos(nID);

	if (pCur->hBmp[nWhichHandle])
		DeleteObject(pCur->hBmp[nWhichHandle]);

	pCur->hBmp[nWhichHandle] = hBmp;	
	if (hWindow)
	{
		InvalidateRect(GetChild(nID)->hWindow, NULL, FALSE);
		UpdateWindow(GetChild(nID)->hWindow);
    }
}	//TToolBar::SetBitmap

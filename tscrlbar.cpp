/*
	TSCRLBAR.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TScrollBar class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TScrollBar::TScrollBar(PTWindowObject pParent, int nID) : TControl(pParent, nID)
{
	SetStyle(WS_VISIBLE, 0);
	nPage = 1;
	nLine = 1;
	EnableThumbTrack(FALSE);
	WndClass.lpszClassName = "SCROLLBAR";
}       //TScrollBar::TScrollBar

//done
WORD TScrollBar::Transfer(LPSTR lpData, TransDir Dir)
{
	PTScrollBarData lpSBData = (PTScrollBarData) lpData;

	switch (Dir)
	{
		case TF_SET:
			SetRange(lpSBData->nMin, lpSBData->nMax);
			SetPos(lpSBData->nPos, FALSE);
			break;

		case TF_GET:
			GetRange(lpSBData->nMin, lpSBData->nMax);
			lpSBData->nPos = GetPos();
	}

	return scrollbarTFSize;
}       //TScrollBar::Transfer

//done
void TScrollBar::SetRange(int nMn, int nMx, BOOL bRedraw)
{
	SetScrollRange(hWindow, SB_CTL, nMn, nMx, bRedraw);
}       //TScrollBar::SetRange

//done
void TScrollBar::GetRange(int &nMin, int &nMax)
{
    GetScrollRange(hWindow, SB_CTL, &nMin, &nMax);
}       //TScrollBar::GetRange

//done
void TScrollBar::SetPos(int nPos, BOOL bRedraw)
{
	int nMin, nMax;

    GetRange(nMin, nMax);
	if (nPos < nMin)
		nPos = nMin;
	else
	if (nPos > nMax)
		nPos = nMax;

	SetScrollPos(hWindow, SB_CTL, nPos, bRedraw);
}       //TScrollBar::SetPos

//done
int TScrollBar::GetPos()
{
	return GetScrollPos(hWindow, SB_CTL);
}       //TScrollBar::GetPos

//done
void TScrollBar::SetPage(int nPg)
{
	nPage = nPg;
}       //TScrollBar::SetPage

//done
void TScrollBar::SetLine(int nL)
{
	nLine = nL;
}       //TScrollBar::SetLine

//done
void TScrollBar::DeltaScroll(int nDelta)
{
	int nPos = GetPos();

    SetPos(nPos + nDelta);
}       //TScrollBar::DeltaScroll

//done
void TScrollBar::DoScroll(RTMessage Msg)
{
	switch (Msg.wParam)
	{
		case SB_LINEDOWN:
			LineDown();
			break;

		case SB_LINEUP:
			LineUp();
			break;

		case SB_PAGEDOWN:
			PageDown();
			break;

		case SB_PAGEUP:
			PageUp();
			break;

		case SB_THUMBTRACK:
			if (bThumbTrackEnabled)
				ThumbTrack(Msg);
			break;

		case SB_THUMBPOSITION:
		    ThumbPosition(Msg);
			break;

		case SB_ENDSCROLL:
        	EndScroll();
	}
}       //TScrollBar::DoScroll

//done
void TScrollBar::PageUp()
{
	SetPos(GetPos() - nPage);
}       //TScrollBar::PageUp

//done
void TScrollBar::PageDown()
{
	SetPos(GetPos() + nPage);
}       //TScrollBar::PageDown

//done
void TScrollBar::LineUp()
{
	SetPos(GetPos() - nLine);
}       //TScrollBar::LineUp

//done
void TScrollBar::LineDown()
{
	SetPos(GetPos() + nLine);
}       //TScrollBar::LineDown

//done
void TScrollBar::ThumbPosition(RTMessage Msg)
{
	SetPos(Msg.LP.Lo);
}       //TScrollBar::ThumbPosition

//done
void TScrollBar::ThumbTrack(RTMessage Msg)
{
	ThumbPosition(Msg);
}       //TScrollBar::ThumbTrack

//done
void TScrollBar::EndScroll()
{
}	//TScrollBar::EndScroll

//done
void TScrollBar::EnableThumbTrack(BOOL bEnable)
{
	bThumbTrackEnabled = bEnable;
}       //TScrollBar::EnableThumbTrack

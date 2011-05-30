/*
	TEDIT.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TEdit class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TEdit::TEdit(PTWindowObject pParent, int nID, WORD cbDataSize) : TStatic(pParent, nID, cbDataSize)
{
	SetStyle(WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT, 0);
    static_editTFSize = cbDataSize;
	WndClass.lpszClassName = "EDIT";
}	//TEdit::TEdit

//done
BOOL TEdit::CanUndo()
{
	return (BOOL) SendMessage(hWindow, EM_CANUNDO, 0, 0);
}	//TEdit::CanUndo

//done
void TEdit::ClearUndoBuffer()
{
	SendMessage(hWindow, EM_EMPTYUNDOBUFFER, 0, 0);
}	//TEdit::ClearUndoBuffer

//done
void TEdit::EnableSoftLineBreak(BOOL bEnable)
{
	SendMessage(hWindow, EM_FMTLINES, (WPARAM) bEnable, 0); 
}	//TEdit::EnableSoftLineBreak

//done
void TEdit::GetLine(int nLine, LPSTR lpszBuf, int cbMaxChars)
{
	*(WORD*) lpszBuf = cbMaxChars - 1;
	lpszBuf[(int) SendMessage(hWindow, EM_GETLINE, (WPARAM) nLine, (LPARAM) lpszBuf)] = 0;
}	//TEdit::GetLine

//done
int TEdit::GetNumLines()
{
	return (int) SendMessage(hWindow, EM_GETLINECOUNT, 0, 0);
}	//TEdit::GetNumLines

//done
BOOL TEdit::IsModified()
{
	return (BOOL) SendMessage(hWindow, EM_GETMODIFY, 0, 0);
}	//TEdit::IsModified

//done
void TEdit::SetModified(BOOL bOn)
{
	SendMessage(hWindow, EM_SETMODIFY, (WPARAM) bOn, 0);
}	//TEdit::SetModified

//done
char TEdit::GetPasswordChar()
{
	return (char) SendMessage(hWindow, EM_GETPASSWORDCHAR, 0, 0);
}	//TEdit::GetPasswordChar

//done
void TEdit::SetPasswordChar(char c)
{
	SendMessage(hWindow, EM_SETPASSWORDCHAR, (WPARAM) c, 0); 
}	//TEdit::SetPasswordChar

//done
DWORD TEdit::GetSel(LPSTR lpszBuf, int cbBufLen)
{
	LRESULT lResult = SendMessage(hWindow, EM_GETSEL, 0, 0);
    WORD cbCharsInSel = HIWORD(lResult) - LOWORD(lResult);

    if (lpszBuf)
	{
		HLOCAL hText = (HLOCAL) SendMessage(hWindow, EM_GETHANDLE, 0, 0);
		PSTR p = (PSTR) LocalLock(hText);

		lstrcpyn(lpszBuf, &p[LOWORD(lResult)], (cbCharsInSel > cbBufLen - 1) ? cbBufLen - 1 : cbCharsInSel);
		LocalUnlock(hText);
	}

	return lResult;
}	//TEdit::GetSel

//done
void TEdit::LimitText(int cbMaxChars)
{
	SendMessage(hWindow, EM_LIMITTEXT, (WPARAM) cbMaxChars, 0);
}	//TEdit::LimitText

//done
int TEdit::GetLineFromChar(int nCharIndex)
{
	return (int) SendMessage(hWindow, EM_LINEFROMCHAR, (WPARAM) nCharIndex, 0);
}	//TEdit::GetLineFromChar

//done
int TEdit::GetLineIndex(int nLine)
{
	return (int) SendMessage(hWindow, EM_LINEINDEX, (WPARAM) nLine, 0);
}	//TEdit::GetLineIndex

//done
int TEdit::GetLineLength(int nCharIndex)
{
	return (int) SendMessage(hWindow, EM_LINELENGTH, (WPARAM) nCharIndex, 0); 
}	//TEdit::GetLineLength

//done
void TEdit::Scroll(int nX, int nY)
{
	SendMessage(hWindow, EM_LINESCROLL, 0, MAKELPARAM(nY, nX));
}	//TEdit::Scroll

//done
void TEdit::ReplaceSel(LPCSTR lpszBuf)
{
	SendMessage(hWindow, EM_REPLACESEL, 0, (LPARAM) lpszBuf);
}	//TEdit::ReplaceSel

//done
void TEdit::SetReadOnly(BOOL bReadOnly)
{
	SendMessage(hWindow, EM_SETREADONLY, (WPARAM) bReadOnly, 0);
}	//TEdit::SetReadOnly

//done
void TEdit::SetSel(int nStart, int nEnd)
{
	SendMessage(hWindow, EM_SETSEL, (WPARAM) FALSE, MAKELPARAM(nStart, nEnd));  
}	//TEdit::SetSel

//done
void TEdit::SetTabs(const int *pTabs, int nNumTabs)
{
	SendMessage(hWindow, EM_SETTABSTOPS, (WPARAM) nNumTabs, (LPARAM) pTabs);
}	//TEdit::SetTabs

//done
void TEdit::Undo()
{
	SendMessage(hWindow, EM_UNDO, 0, 0); 
}	//TEdit::Undo

//done
void TEdit::Cut()
{
	SendMessage(hWindow, WM_CUT, 0, 0);
}	//TEdit::Cut

//done
void TEdit::Copy()
{
	SendMessage(hWindow, WM_COPY, 0, 0);
}	//TEdit::Copy

//done
void TEdit::Paste()
{
	SendMessage(hWindow, WM_PASTE, 0, 0);
}	//TEdit::Paste

//done
void TEdit::ClearSel()
{
	SendMessage(hWindow, WM_CLEAR, 0, 0);
}	//TEdit::ClearSel

//done
void TEdit::GetBoundRect(LPRECT lpRect)
{
	SendMessage(hWindow, EM_GETRECT, 0, (LPARAM) lpRect); 
}	//TEdit::GetBoundRect

//done
void TEdit::SetBoundRect(const RECT *lpRect, BOOL bRedraw)
{
	SendMessage(hWindow, bRedraw ? EM_SETRECT : EM_SETRECTNP, 0, (LPARAM) lpRect);
}	//TEdit::SetBoundRect

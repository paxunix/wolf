/*
	TLISTBOX.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TListBox class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#include <mem.h>

//done
TListBox::TListBox(PTWindowObject pParent, int nID) : TControl(pParent, nID)
{
	SetStyle(WS_VISIBLE | LBS_STANDARD, 0);
	bIsListBox = TRUE;
	WndClass.lpszClassName = "LISTBOX";
}	//TListBox::TListBox

//done
WORD TListBox::Transfer(LPSTR, TransDir)
{
    return 0;
}	//TListBox::Transfer

//done
int TListBox::AddString(LPCSTR lpszString)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_ADDSTRING : CB_ADDSTRING, 0, (LPARAM) lpszString);
}	//TListBox::AddString

//done
void TListBox::ClearList()
{
	SendMessage(hWindow, bIsListBox ? LB_RESETCONTENT : CB_RESETCONTENT, 0, 0);
}	//TListBox::ClearList

//done
int TListBox::DeleteString(int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_DELETESTRING : CB_DELETESTRING, (WPARAM) nIndex, 0);
}	//TListBox::DeleteString

//done
int TListBox::FindExactString(LPCSTR lpszString, int nStartIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_FINDSTRINGEXACT : CB_FINDSTRINGEXACT, (WPARAM) nStartIndex, (LPARAM) lpszString);
}	//TListBox::FindExactString

//done
int TListBox::FindString(LPCSTR lpszString, int nStartIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_FINDSTRING : CB_FINDSTRING, (WPARAM) nStartIndex, (LPARAM) lpszString);
}	//TListBox::FindString

//done
int TListBox::GetCount()
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_GETCOUNT : CB_GETCOUNT, 0, 0);
}	//TListBox::GetCount

//done
int TListBox::GetSelCount()
{
	return (int) SendMessage(hWindow, LB_GETSELCOUNT, 0, 0);
}	//TListBox::GetSelCount

//done
int TListBox::GetSelIndex()
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_GETCURSEL : CB_GETCURSEL, 0, 0);
}	//TListBox::GetSelIndex

//done
int TListBox::GetSelIndices(int *pIndices, int cbMaxItems)
{
	return (int) SendMessage(hWindow, LB_GETSELITEMS, (WPARAM) cbMaxItems, (LPARAM) pIndices);
}	//TListBox::GetSelIndices

//done
int TListBox::GetSelString(LPSTR lpszString, int cbMaxChars)
{
	return GetString(lpszString, GetSelIndex(), cbMaxChars);
}	//TListBox::GetSelString

//done
int TListBox::GetSelStrings(LPSTR *lpStrs, int cbMaxCount, int cbMaxChars)
{
	int *pIndices;

	pIndices = new int[cbMaxCount];
	int nCount = GetSelIndices(pIndices, cbMaxCount);
	for (int i=0; i<nCount; i++)
		GetString(lpStrs[i], i, cbMaxChars);

	return nCount; 
}	//TListBox::GetSelStrings

//done
int TListBox::GetString(LPSTR lpszString, int nIndex, int cbMaxChars)
{
	int nLen = GetStringLen(nIndex) + 1;

	if (nLen == 1)
	{
		lpszString[0] = 0;
		return 0;
	}

	char *pszStr = new char[nLen];
	SendMessage(hWindow, bIsListBox ? LB_GETTEXT : CB_GETLBTEXT, (WPARAM) nIndex, (LPARAM) pszStr);

	if (cbMaxChars < nLen)	//string is longer than given buffer
		_fmemcpy(lpszString, pszStr, cbMaxChars);
	else
		_fmemcpy(lpszString, pszStr, nLen);

	delete[] pszStr;
    lpszString[nLen - 1] = 0;	//make sure there's a NULL
	return nLen > cbMaxChars ? cbMaxChars - 1 : nLen - 1;
}	//TListBox::GetString

//done
int TListBox::GetStringLen(int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_GETTEXTLEN : CB_GETLBTEXTLEN, (WPARAM) nIndex, 0);
}	//TListBox::GetStringLen
      
//done
int TListBox::InsertString(LPCSTR lpszString, int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_INSERTSTRING : CB_INSERTSTRING, (WPARAM) nIndex, (LPARAM) lpszString);	
}	//TListBox::InsertString

//done
int TListBox::SetSelIndex(int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_SETCURSEL : CB_SETCURSEL, (WPARAM) nIndex, 0); 
}	//TListBox::SetSelIndex

//done
void TListBox::SetSelIndices(const int *pIndices, int nNumIndices, BOOL bSelect)
{
	for (int i=0; i<nNumIndices; i++)
		SendMessage(hWindow, LB_SETSEL, (WPARAM) bSelect, MAKELPARAM(pIndices[i], 0));
}	//TListBox::SetSelIndices

//done
int TListBox::SetSelString(LPCSTR lpszString, int nStartIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_SELECTSTRING : CB_SELECTSTRING, (WPARAM) nStartIndex, (LPARAM) lpszString); 
}	//TListBox::SetSelString

//done
void TListBox::SetSelStrings(LPCSTR *lpStrings, int nNumStrings)
{
	for (int i=0; i<nNumStrings; i++)
		SetSelString(lpStrings[i], -1);
}	//TListBox::SetSelStrings

//done
int TListBox::GetXExtent()
{
	return (int) SendMessage(hWindow, LB_GETHORIZONTALEXTENT, 0, 0);
}	//TListBox::GetXExtent

//done
void TListBox::SetXExtent(int nExtent)
{
	SendMessage(hWindow, LB_SETHORIZONTALEXTENT, (WPARAM) nExtent, 0);
}	//TListBox::SetXExtent

//done
int TListBox::GetItemHeight(int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_GETITEMHEIGHT : CB_GETITEMHEIGHT, (WPARAM) nIndex, 0);
}	//TListBox::GetItemHeight

//done
int TListBox::SetItemHeight(int nHeight, int nIndex)
{
	return (int) SendMessage(hWindow, bIsListBox ? LB_SETITEMHEIGHT : CB_SETITEMHEIGHT, (WPARAM) nIndex, MAKELPARAM(nHeight, 0));
}	//TListBox::SetItemHeight

/*
	TSTATIC.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TStatic class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

//done
TStatic::TStatic(PTWindowObject pParent, int nID, WORD cbDataSize) : TControl(pParent, nID)
{
	SetStyle(WS_VISIBLE | SS_LEFT | SS_NOPREFIX, 0);
	static_editTFSize = cbDataSize;
	WndClass.lpszClassName = "STATIC";
}	//TStatic::TStatic

//done
WORD TStatic::Transfer(LPSTR lpData, TransDir Dir)
{
	if (static_editTFSize)
	   	switch (Dir)
		{
			case TF_SET:
                if (WndClass.lpszClassName[0] == 'E')	//it's an edit control
					SendMessage(hWindow, EM_LIMITTEXT, (WPARAM) static_editTFSize - 1, 0);

				SetText(lpData);
                if (WndClass.lpszClassName[0] == 'E')	//it's an edit control
					SendMessage(hWindow, EM_SETSEL, 0, MAKELONG(0, -1));
				break;
	
			case TF_GET:
				GetText(lpData, static_editTFSize);
		};

	return static_editTFSize;	//skip terminating NULL
}	//TStatic::Transfer

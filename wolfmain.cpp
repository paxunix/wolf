/*
	WOLFMAIN.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Main source code module for WOLF.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

PTApplication pTempApp;	//a temporary so pApplication data members are available
						//in main window's constructor.
HINSTANCE hLibInst;		//global handle to DLL instance needed by TFileDialog and TInputDialog
PTWindowObject pGlobalWindowObject;		//used to connect window object with interface element

#pragma codestring "WOLF - Windows Object Library/Framework"
#pragma codestring "Copyright (c) 1994  Shawn Halpenny"
#pragma codestring "All rights reserved."

//done
int CALLBACK LibMain(HINSTANCE hInst, WORD, WORD wHeapSize, LPSTR)	
{
	if (wHeapSize > 0)
		UnlockData(0);

	hLibInst = hInst;
	return 1;
}	//LibMain

//done
LONG UMP(RTMessage Msg)
{
	PTWindowObject pWindowObject;

	pWindowObject = (PTWindowObject) MAKELP(GetProp(Msg.hWindow, "WolfS"), GetProp(Msg.hWindow, "WolfO"));
	if (pGlobalWindowObject && !pWindowObject)
	{	//don't assign the wrong properties to the window for whom they are intended
 	   	pWindowObject = pGlobalWindowObject;
		pWindowObject->hWindow = Msg.hWindow;
		pWindowObject->SetProperties();
		SetGlobalWindowObject(NULL);
	}

	return pWindowObject->DispatchAMessage(Msg.wMsg + WM_FIRST, Msg);
}	//UMP

//done
void _export SetGlobalWindowObject(PTWindowObject pObj)
{
	pGlobalWindowObject = pObj;
}	//SetGlobalWindowObject

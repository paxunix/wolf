/*
	TAPP.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TApplication class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#ifndef __STDIO_H
#include <stdio.h>
#endif

extern PTApplication pTempApp;

//done
TApplication::TApplication(LPCSTR lpszAClassName, HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLn, int nShow)
{
	lpszClassName = lpszAClassName;
	hInstance = hInst;

	pTempApp = this;	//see TWindowObject constructor for explanation

	hPrevInstance = hPrevInst;
	lpszCmdLine = lpszCmdLn;
	nCmdShow = nShow;
	nStatus = 0;
	pMainWindow = NULL;
	hAccel = 0;
}	//TApplication::TApplication

//done
TApplication::~TApplication()
{
	delete pMainWindow;
}	//TApplication::~TApplication

//done
void TApplication::Run()
{
	MSG AMsg;

	if (nStatus != -1)
	{
		InitMainWindow();
		pMainWindow->WndClass.lpszClassName = lpszClassName;
		pMainWindow->Create();
		pMainWindow->Show(nCmdShow);

		do {
			if (PeekMessage(&AMsg, 0, 0, 0, PM_NOREMOVE))
			{
				if (!GetMessage(&AMsg, 0, 0, 0))
					break;

				if (!ProcessMsgs(AMsg))
				{
                	TranslateMessage(&AMsg);
					DispatchMessage(&AMsg);
                }
			}
			else
				Idle();
		} while (TRUE);

		nStatus = (int) AMsg.wParam;
	}
}	//TApplication::Run

//done
void TApplication::AssignAccel(int nAccelID)
{
	AssignAccel(MAKEINTRESOURCE(nAccelID));
}	//TApplication::AssignAccel

//done
void TApplication::AssignAccel(LPCSTR lpszAccel)
{
	hAccel = LoadAccelerators(hInstance, lpszAccel);
}	//TApplication::AssignAccel

//done
int TApplication::ExecDialog(PTDialog pDialog)
{
	int nRetval;

	pDialog->Create();
	nRetval = pDialog->nStatus;
	delete pDialog;

	return nRetval;
}	//TApplication::ExecDialog

//done
int TApplication::Input(PTWindowObject pParent, LPSTR lpszBuffer, int nBufLen, LPCSTR lpszQuery, ...)
{
	LPSTR lpArgs = (LPSTR) &lpszQuery + sizeof(lpszQuery);
	char *pszBuf = new char[2048];
    int nRetval;

    vsprintf(pszBuf, lpszQuery, lpArgs);
	nRetval = ExecDialog(new TInputDialog(pParent, pszBuf, lpszBuffer, nBufLen));
	delete[] pszBuf;
	return nRetval;
}	//TApplication::Input

//done
int TApplication::FileOpen(PTWindowObject pParent, RFileDlgData ff)
{
	return ExecDialog(new TFileDialog(pParent, FD_OPEN, ff));
}	//TApplication::FileOpen

//done
int TApplication::FileSave(PTWindowObject pParent, RFileDlgData ff)
{
	return ExecDialog(new TFileDialog(pParent, FD_SAVE, ff));
}	//TApplication::FileSave

//done
void TApplication::Idle()
{
}	//TApplication::Idle

//done
int TApplication::MsgBox(PTWindowObject pParent, UINT wStyle, LPCSTR lpszCaption, LPCSTR lpszFormat, ...)
{
    LPSTR lpArgs = (LPSTR) &lpszFormat + sizeof(lpszFormat);
	char *pszBuf = new char[2048];
    int nRetval;

	vsprintf(pszBuf, lpszFormat, lpArgs);
	nRetval = MessageBox(pParent->hWindow, pszBuf, lpszCaption, wStyle);
	delete[] pszBuf;
	return nRetval;
}	//TApplication::MsgBox

//done
BOOL TApplication::ProcessMsgs(MSG &msg)
{
	BOOL bRetval = FALSE;

	if (ProcessDlgMsgs(msg))
		bRetval = TRUE;

	if (ProcessAccels(msg))
		bRetval = TRUE;

	if (ProcessMDIAccels(msg))
		bRetval = TRUE;

	return bRetval;
}	//TApplication::ProcessMsgs

//done
BOOL TApplication::ProcessDlgMsgs(MSG &AMsg)
{
	HWND hParentWnd;
	PTWindowObject pWindow, pParentWindow;

	hParentWnd = GetParent(AMsg.hwnd);
	pWindow = (PTWindowObject) MAKELP(GetProp(AMsg.hwnd, "WolfS"), GetProp(AMsg.hwnd, "WolfO"));
	if (pWindow)
		if (pWindow->IsFlagSet(WB_KBHANDLER))
			if (IsDialogMessage(AMsg.hwnd, &AMsg))
				return TRUE;

	if (hParentWnd)
	{
		pParentWindow = (PTWindowObject) MAKELP(GetProp(hParentWnd, "WolfS"), GetProp(hParentWnd, "WolfO"));
		if (pParentWindow)
			if (pParentWindow->IsFlagSet(WB_KBHANDLER))
				if (IsDialogMessage(hParentWnd, &AMsg))
					return TRUE;
	}

	return FALSE;
}	//TApplication::ProcessDlgMsgs

//done
BOOL TApplication::ProcessAccels(MSG &AMsg)
{
	if (hAccel)
		return TranslateAccelerator(pMainWindow->hWindow, hAccel, &AMsg);
	else
    	return FALSE;
}	//TApplication::ProcessAccels

//done
BOOL TApplication::ProcessMDIAccels(MSG &msg)
{
	if (pMainWindow->Identify() == mdiframeClass)
		return TranslateMDISysAccel(((PTMDIFrame) pMainWindow)->pClient->hWindow, &msg);
	else
    	return FALSE;
}	//TApplication::ProcessMDIAccels

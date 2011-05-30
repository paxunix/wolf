/*
	TFILEDLG.CPP
	Copyright (c) 1994  Shawn Halpenny
	All rights reserved.

	Source code module for TFileDialog class.
*/

#ifndef __WOLF_H
#include <wolf.h>
#endif

#ifndef __STDIO_H
#include <stdio.h>
#endif

#ifndef __STRING_H
#include <string.h>
#endif

#ifndef __IO_H
#include <io.h>
#endif

#ifndef __COMMDLG_H
#include <commdlg.h>
#endif

extern HINSTANCE hLibInst;

//This type definition must match that of the OPENFILENAME structure
//field lpfnHook.
typedef UINT (CALLBACK *OFNHOOKPROC)(HWND, UINT, WPARAM, LPARAM);

//done
TFileDialog::TFileDialog(PTWindowObject pParent, int nT, RFileDlgData fdd) : TDialog(pParent, ID_FILEDLG)
{
    SetFlags(WB_TRANSFER, FALSE);
    SetTransferBuffer(NULL);
	nType = nT;
    pfdd = &fdd;

	_fmemset(&ofn, 0, sizeof(ofn));

	//Correct the filter string
	for (int i = 0; fdd.lpszFilters[i]; i++)
		if (fdd.lpszFilters[i] == '|')
			fdd.lpszFilters[i] = 0;

	ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = pParent->hWindow;
    ofn.hInstance = hLibInst;
	ofn.lpstrFilter = fdd.lpszFilters;
	ofn.nFilterIndex = fdd.dwFilterIndex;
	ofn.lpstrFile = fdd.lpszFile;
	ofn.nMaxFile = fdd.dwMaxFile;
	ofn.lpstrFileTitle = fdd.lpszFileTitle;
	ofn.nMaxFileTitle = fdd.dwMaxFileTitle;
	ofn.lpstrInitialDir = fdd.lpszInitialDir;
	ofn.Flags = OFN_ENABLEHOOK | OFN_ENABLETEMPLATE | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	if (nT == FD_OPEN)
	{
    	ofn.lpstrTitle = "Open";
    	ofn.Flags |= OFN_FILEMUSTEXIST;
	}
	else
	{
		ofn.lpstrTitle = "Save As";
        ofn.Flags |= OFN_OVERWRITEPROMPT;
    }

	ofn.lpstrDefExt = fdd.lpszDefExt;
	ofn.lpfnHook = (OFNHOOKPROC) TDialog::DlgProc;
    ofn.lpTemplateName = MAKEINTRESOURCE(nID);
}	//TFileDialog::TFileDialog

//done
BOOL TFileDialog::Create()
{
	SetGlobalWindowObject(this);

	if (nType == FD_OPEN)
		nStatus = GetOpenFileName(&ofn);
	else
		nStatus = GetSaveFileName(&ofn);

	pfdd->uFileOffset = ofn.nFileOffset; 
    pfdd->uExtOffset = ofn.nFileExtension; 
    return (CommDlgExtendedError() == 0);
}	//TFileDialog::Create

//done
void TFileDialog::Destroy(int)
{
	//just return, because hook functions can't call EndDialog for common dialogs.
}	//TFileDialog::Destroy

void TFileDialog::RereadFiles()
{
	//"Double-click" the list box.
    SendMessage(hWindow, WM_COMMAND, 1121, MAKELPARAM(GetDlgItem(hWindow, 1121), LBN_DBLCLK));
}	//TFileDialog::RereadFiles

//done
void TFileDialog::IDDelete(RTMessage)
{
    char szBuf[128];

	GetDlgItemText(hWindow, 1088, szBuf, 100);
	if (szBuf[lstrlen(szBuf)-1] != '\\')
		lstrcat(szBuf, "\\");

	GetDlgItemText(hWindow, 1152, &szBuf[0] + lstrlen(szBuf), 27); 
	if (unlink(szBuf))
		MessageBox(hWindow, "Invalid path or file name.", "File Delete Error", MB_OK | MB_ICONEXCLAMATION);

	RereadFiles();
}	//TFileDialog::IDDelete

//done
void TFileDialog::IDRename(RTMessage)
{
	char szBuf[128], szNewName[128];

	GetDlgItemText(hWindow, 1088, szBuf, 100);
	if (szBuf[lstrlen(szBuf)-1] != '\\')
		lstrcat(szBuf, "\\");

    lstrcpy(szNewName, szBuf);
	GetDlgItemText(hWindow, 1152, &szBuf[0] + lstrlen(szBuf), 27); 
	if (pApplication->Input(this, szNewName, 128, "Rename '%s' to:", szBuf) == IDOK)
	{
		if (rename(szBuf, szNewName) == -1)
			MessageBox(hWindow, "Invalid path or file name.", "File Rename Error", MB_OK | MB_ICONEXCLAMATION);
		else
			RereadFiles();
	}
}	//TFileDialog::IDRename

//done
void TFileDialog::IDCopy(RTMessage)
{
	char szBuf[128], szNewName[128]="";
	HFILE hOldFile, hNewFile;
	int nNumBytes;
	OFSTRUCT ofs;
	HANDLE hBufferMem;
	LPSTR lpszBytes;
    struct ftime ftime;

	GetDlgItemText(hWindow, 1088, szBuf, 100);
	if (szBuf[lstrlen(szBuf)-1] != '\\')
		lstrcat(szBuf, "\\");

	GetDlgItemText(hWindow, 1152, &szBuf[0] + lstrlen(szBuf), 27); 
	if (pApplication->Input(this, szNewName, 128, "Copy '%s' to:", szBuf) == IDOK)
	{
		hOldFile = OpenFile(szBuf, &ofs, OF_READ);
		hNewFile = OpenFile(szNewName, &ofs, OF_CREATE);
		if (hOldFile == HFILE_ERROR || hNewFile == HFILE_ERROR)
		{
L3:			MessageBox(hWindow, "Invalid path or file name.", "File Copy Error", MB_OK | MB_ICONEXCLAMATION);
			goto L2;
		}

		hBufferMem = GlobalAlloc(GHND, 16384);
		if (!hBufferMem)
		{
			MessageBox(hWindow, "Out of global memory.", "Error!", MB_OK | MB_ICONEXCLAMATION);
			goto L2;
		}
		else
		{
			MSG msg;

            SetCursor(LoadCursor(NULL, IDC_WAIT));
			lpszBytes = (LPSTR) GlobalLock(hBufferMem);
			while ((nNumBytes = _lread(hOldFile, lpszBytes, 16384)) != 0)
			{
				PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE);
				_lwrite(hNewFile, lpszBytes, nNumBytes);
			};

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			GlobalUnlock(hBufferMem);
			GlobalFree(hBufferMem);

			getftime(hOldFile, &ftime);
			setftime(hNewFile, &ftime);

L2:			_lclose(hOldFile);
			_lclose(hNewFile);
		}
	}

    RereadFiles();
}	//TFileDialog::IDCopy

void TFileDialog::Okay(RTMessage Msg)
{
	Msg.lResult = 0;	//have to let the common dlg proc process Okay and Cancel
}	//TFileDialog::Okay

void TFileDialog::Cancel(RTMessage Msg)
{
	Msg.lResult = 0;
}	//TFileDialog::Cancel

void TFileDialog::WMCommand(RTMessage Msg)
{
	if (Msg.wParam < 2000)	//all my additional control IDs are above 1999
		Msg.lResult = 0;	//standard processing
	else
		TWindowObject::WMCommand(Msg);
}	//TFileDialog::WMCommand

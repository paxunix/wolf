/*
	WOLF.H
	Copyright (c) 1994  Shawn Halpenny
    All rights reserved.

	Header file for WOLF.
*/

#ifndef __WOLF_H
#define __WOLF_H

//want strict Windows compilation under version 3.1
#define STRICT
#define WINVER 0x30a

//Include files
#ifndef __WINDOWS_H
#include <windows.h>
#endif

#ifndef ___DEFS_H
#include <_defs.h>
#endif

#ifndef __DLISTIMP_H
#include <dlistimp.h>
#endif

#ifndef __COMMDLG_H
#include <commdlg.h>
#endif

//Facilitate dispatching messages
#define WM_COUNT 0x8000
#define WM_FIRST 0x0000
#define WM_INTERNAL 0x7F00

#define ID_COUNT 0x1000
#define ID_FIRST 0x8000
#define ID_INTERNAL 0x8F00
#define ID_FIRSTMDICHILD (ID_INTERNAL+1)

#define CM_COUNT 0x6000
#define CM_FIRST 0xA000
#define CM_INTERNAL 0xFF00

//Dialog templates specific to WOLF
#define ID_INPUTDLG 32000
#define ID_FILEDLG 32001

//File dialog types
#define FD_OPEN 1
#define FD_SAVE 2

//Transfer buffer mechanism
enum TransDir { TF_SET, TF_GET, TF_SIZE };

//WOLF class identifiers
enum WOLFid { dontCare, mdiclientClass, mdiframeClass, dialogClass, modelessdialogClass, controlClass };
                                       
//Bit flag masks                    
#define WB_AUTOCREATE	0x00000001
#define WB_TRANSFER		0x00000002
#define WB_SYSTEMCLASS	0x00000004
#define WB_KBHANDLER	0x00000008

_CLASSDEF(TMessage)
struct TMessage
{
	HWND hWindow;
	UINT wMsg;

	union
    {
		WORD wParam;
		struct
		{
			BYTE Lo;
			BYTE Hi;
        } WP;
    };

	union
    {
		DWORD lParam;
		struct tagLP
		{
			WORD Lo;
			WORD Hi;
		} LP;
    };

	LONG lResult;
};

_CLASSDEF(TWindowObject)
_CLASSDEF(TWindow)
_CLASSDEF(TMDIFrame)
_CLASSDEF(TMDIClient)
_CLASSDEF(TMDIChild)
_CLASSDEF(TDialog)
_CLASSDEF(TModelessDialog)
_CLASSDEF(TControl)
_CLASSDEF(TPushButton)
_CLASSDEF(TGroupBox)
_CLASSDEF(TCheckBox)
_CLASSDEF(TRadioButton)
_CLASSDEF(TStatic)
_CLASSDEF(TEdit)
_CLASSDEF(TListBox)
_CLASSDEF(TComboBox)
_CLASSDEF(TScrollBar)
_CLASSDEF(TInputDialog)
_CLASSDEF(TFileDialog)
_CLASSDEF(TApplication)
_CLASSDEF(TToolBar)

class _CLASSTYPE TWindowObject
{
protected:
	LPSTR lpTransferBuffer;
	DWORD dwFlags;

public:
	BI_DoubleListImp<PTWindowObject> ChildList;
	PTApplication pApplication;
	PTWindowObject pParent;
	HWND hWindow;
	int x, y, cx, cy;
	DWORD dwStyle;
    DWORD dwExStyle;
	LPCSTR lpszWindowText;
	int nID;
	WNDCLASS WndClass;

	TWindowObject(PTWindowObject, LPCSTR, int=0);
	virtual ~TWindowObject();
	void SetProperties();
	void RemoveProperties();
	virtual WOLFid Identify() = 0;
	BOOL IsFlagSet(DWORD);
    void SetFlags(DWORD, BOOL);
	void SetTransferBuffer(LPSTR);
	BOOL CreateChildren();
	virtual void WMCommand(RTMessage) = [WM_FIRST + WM_COMMAND];
    virtual void WMClose(RTMessage) = [WM_FIRST + WM_CLOSE];
	virtual void WMDestroy(RTMessage) = [WM_FIRST + WM_DESTROY];
    virtual void WMNCDestroy(RTMessage) = [WM_FIRST + WM_NCDESTROY];
	virtual void WMHScroll(RTMessage) = [WM_FIRST + WM_HSCROLL];
	virtual void WMVScroll(RTMessage) = [WM_FIRST + WM_VSCROLL];
	virtual void WMQueryEndSession(RTMessage) = [WM_FIRST + WM_QUERYENDSESSION];
    virtual BOOL Create();
	virtual BOOL Register();
	PTWindowObject GetChild(int);
	void DoTransfer(TransDir);
	virtual BOOL CanClose();
	virtual LONG DispatchAMessage(UINT, RTMessage);
	virtual LONG DefMsgHandler(RTMessage);
	void Show(int=SW_SHOWNORMAL);
	virtual WORD Transfer(LPSTR, TransDir) = 0;
	void SetText(LPCSTR);
	void GetText(LPSTR, int);
	int GetTextLen();
	void Enable(BOOL);
	void SetDimensions(int, int, int, int);
	void GetDimensions();
	void SetStyle(DWORD, DWORD);
	void GetStyle();
	void SetFocus();
};

class _CLASSTYPE TWindow : public TWindowObject
{
protected:
	static LRESULT CALLBACK _export StandardWndProc(HWND, UINT, WPARAM, LPARAM);

public:
	TWindow(PTWindowObject, LPCSTR, int=0);
    ~TWindow();
	WORD Transfer(LPSTR, TransDir)
	{
		return 0;
	}

	virtual void WMPaint(RTMessage) = [WM_FIRST + WM_PAINT];
	virtual void Paint(HDC, PAINTSTRUCT _FAR &);
	WOLFid Identify() { return dontCare; }
	void AssignMenu(int);
    void AssignMenu(LPCSTR);
	void AssignIcon(int);
    void AssignIcon(LPCSTR);
	void AssignCursor(int);
    void AssignCursor(LPCSTR);
	virtual void WMCreate(RTMessage) = [WM_FIRST + WM_CREATE];
};

class _CLASSTYPE TMDIFrame : public TWindow
{
	void Init();

public:
	PTMDIClient pClient;
	int nWindowMenuPos;

	TMDIFrame(LPCSTR, int);
	WOLFid Identify() { return mdiframeClass; }
	LONG DefMsgHandler(RTMessage);
	void CloseChildren();
	virtual void WMCreate(RTMessage) = [WM_FIRST + WM_CREATE];
    virtual void WMCommand(RTMessage) = [WM_FIRST + WM_COMMAND];
	BOOL CanClose();
};

class _CLASSTYPE TMDIClient : public TWindow
{
	CLIENTCREATESTRUCT ClientData;

public:

	PTMDIFrame pFrame;

	TMDIClient(PTMDIFrame);
	WOLFid Identify() { return mdiclientClass; }
	BOOL Create();
};

class _CLASSTYPE TMDIChild : public TWindow
{
	BOOL bCreated;
	MDICREATESTRUCT ChildData;

public:
	int nChildMenuPos;

	TMDIChild(PTMDIFrame, LPCSTR);
	LONG DefMsgHandler(RTMessage);
	BOOL Create();
	virtual void WMMDIActivate(RTMessage) = [WM_FIRST + WM_MDIACTIVATE];
	virtual void WMNCDestroy(RTMessage) = [WM_FIRST + WM_NCDESTROY];
};

class _CLASSTYPE TDialog : public TWindow
{
protected:
	static BOOL CALLBACK _export DlgProc(HWND, UINT, WPARAM, LPARAM);

public:
	int nStatus;

	TDialog(PTWindowObject, int);
	WOLFid Identify() { return dialogClass; }
	BOOL Create();
	LONG DefMsgHandler(RTMessage);
	virtual void WMInitDialog(RTMessage) = [WM_FIRST + WM_INITDIALOG];
	virtual LRESULT InitDialog() { return -1; }
	virtual void Okay(RTMessage) = [ID_FIRST + IDOK];
	virtual void Cancel(RTMessage) = [ID_FIRST + IDCANCEL];
	virtual void Destroy(int);
	virtual void WMClose(RTMessage) = [WM_FIRST + WM_CLOSE];
	WORD GetDefault();
	void SetDefault(int);
};

class _CLASSTYPE TModelessDialog : public TDialog
{
protected:
	DLGPROC lpfnDlgProc;

public:
	TModelessDialog(PTWindowObject, int);
	WOLFid Identify() { return modelessdialogClass; }
	BOOL Create();
	void Destroy(int);
};

class _CLASSTYPE TControl : public TWindow
{
protected:
	WNDPROC lpfnOldWndProc;

public:
	TControl(PTWindowObject, int);
	WOLFid Identify() { return controlClass; }
	virtual void WMPaint(RTMessage) = [WM_FIRST + WM_PAINT];
	WORD Transfer(LPSTR, TransDir) = 0;
	BOOL Create();
	void SetRedraw(BOOL);
	void SubClass();
	void UnSubClass();
	LONG DefMsgHandler(RTMessage);
};

class _CLASSTYPE TPushButton : public TControl
{
public:
	TPushButton(PTWindowObject, int);
	WORD Transfer(LPSTR, TransDir)
	{
		return 0;
	}
};

class _CLASSTYPE TGroupBox : public TPushButton
{
public:
	TGroupBox(PTWindowObject, int);
};

#define checkboxTFSize sizeof(UINT)
#define BF_UNCHECKED 0
#define BF_CHECKED 1
#define BF_GRAYED 2 

class _CLASSTYPE TCheckBox : public TPushButton
{
public:
	TCheckBox(PTWindowObject, int);
	WORD Transfer(LPSTR, TransDir);
	virtual int GetCheck();
	virtual void SetCheck(int);
};

#define radiobuttonTFSize sizeof(UINT)

class _CLASSTYPE TRadioButton : public TCheckBox
{
protected:
	int nStartID, nEndID;

public:
	TRadioButton(PTWindowObject, int, int, int);
	int GetCheck();
	void SetCheck(int);
};

class _CLASSTYPE TStatic : public TControl
{
protected:
	WORD static_editTFSize;

public:
	TStatic(PTWindowObject, int, WORD);
	WORD Transfer(LPSTR, TransDir);
};

class _CLASSTYPE TEdit : public TStatic
{
public:
	TEdit(PTWindowObject, int, WORD);
	BOOL CanUndo();
	void ClearUndoBuffer();
	void EnableSoftLineBreak(BOOL);
	void GetLine(int, LPSTR, int);
	int GetNumLines();
	BOOL IsModified();
	void SetModified(BOOL);
	char GetPasswordChar();
	void SetPasswordChar(char);
	DWORD GetSel(LPSTR=NULL, int=0);
	void LimitText(int);
	int GetLineFromChar(int);
	int GetLineIndex(int);
	int GetLineLength(int);
    void Scroll(int, int);
	void ReplaceSel(LPCSTR);
	void SetReadOnly(BOOL);
	void SetSel(int, int);
	void SetTabs(const int _FAR *, int);
	void Undo();
	void Cut();
	void Copy();
	void Paste();
	void ClearSel();
	void GetBoundRect(LPRECT);
    void SetBoundRect(const RECT _FAR *, BOOL);
};

class _CLASSTYPE TListBox : public TControl
{
protected:
	BOOL bIsListBox;

public:
	TListBox(PTWindowObject, int);
	WORD Transfer(LPSTR, TransDir);
	int AddString(LPCSTR);
    void ClearList();
	int DeleteString(int);
	int FindExactString(LPCSTR, int);
	int FindString(LPCSTR, int);
	int GetCount();
	int GetSelCount();
	int GetSelIndex();
	int GetSelIndices(int _FAR *, int);
	int GetSelString(LPSTR, int);
	int GetSelStrings(LPSTR*, int, int);
	int GetString(LPSTR, int, int);
	int GetStringLen(int);
	int InsertString(LPCSTR, int);
	int SetSelIndex(int);
	void SetSelIndices(const int _FAR *, int, BOOL);
	int SetSelString(LPCSTR, int);
	void SetSelStrings(LPCSTR*, int);
	int GetXExtent();
	void SetXExtent(int);
	int GetItemHeight(int=0);
    int SetItemHeight(int,int=0);
};

class _CLASSTYPE TComboBox : public TListBox
{
public:
	TComboBox(PTWindowObject, int);
	int GetEditSel(int _FAR &, int _FAR &);
	int SetEditSel(int, int);
	int ShowList(BOOL);
};

#define scrollbarTFSize sizeof(TScrollBarData)

_CLASSDEF(TScrollBarData)
struct TScrollBarData
{
	int nMin;
	int nMax;
	int nPos;
};

class _CLASSTYPE TScrollBar : public TControl
{
protected:
	int nPage;
	int nLine;
	BOOL bThumbTrackEnabled;

public:
	TScrollBar(PTWindowObject, int);
	WORD Transfer(LPSTR, TransDir);
	void SetRange(int, int, BOOL=FALSE);
	void GetRange(int _FAR &, int _FAR &);
	void SetPos(int, BOOL=TRUE);
	int GetPos();
	void SetPage(int);
	void SetLine(int);
	void DeltaScroll(int);
	void DoScroll(RTMessage);
    void EnableThumbTrack(BOOL);
	virtual void PageUp();
	virtual void PageDown();
	virtual void LineUp();
	virtual void LineDown();
	virtual void ThumbPosition(RTMessage);
	virtual void ThumbTrack(RTMessage);
    virtual void EndScroll();
};

class _CLASSTYPE TInputDialog : public TDialog
{
protected:
	LPCSTR lpszQuery;
	LPSTR lpszBuffer;
	int nBufLen;

public:
	TInputDialog(PTWindowObject, LPCSTR, LPSTR, int);
	BOOL Create();
	LRESULT InitDialog();
};

_CLASSDEF(FileDlgData)
struct FileDlgData 
{
	LPSTR lpszFilters;
	DWORD dwFilterIndex;
	LPSTR lpszFile;
	DWORD dwMaxFile;
	LPSTR lpszFileTitle;
	DWORD dwMaxFileTitle;
	LPCSTR lpszInitialDir;
	UINT uFileOffset;
	UINT uExtOffset;
	LPCSTR lpszDefExt;		
};

class _CLASSTYPE TFileDialog : public TDialog
{
protected:
	OPENFILENAME ofn;
	PFileDlgData pfdd;
	int nType;

    void RereadFiles();

public:
	TFileDialog(PTWindowObject, int, RFileDlgData);
	BOOL Create();
	virtual void IDDelete(RTMessage) = [ID_FIRST + 2000];
	virtual void IDRename(RTMessage) = [ID_FIRST + 2001];
	virtual void IDCopy(RTMessage) = [ID_FIRST + 2002];
	virtual void Okay(RTMessage) = [ID_FIRST + IDOK];
	virtual void Cancel(RTMessage) = [ID_FIRST + IDCANCEL];
    virtual void WMCommand(RTMessage) = [WM_FIRST + WM_COMMAND];
	void Destroy(int);
};

class _CLASSTYPE TApplication
{
protected:
	int nCmdShow;
	HACCEL hAccel;

public:
	PTWindowObject pMainWindow;
	LPCSTR lpszClassName;
	HINSTANCE hInstance, hPrevInstance;
	LPSTR lpszCmdLine;
	int nStatus;

	TApplication(LPCSTR, HINSTANCE, HINSTANCE, LPSTR, int);
	virtual ~TApplication();
	virtual void InitMainWindow() = 0;
	void Run();
	void AssignAccel(int);
	void AssignAccel(LPCSTR);
	int ExecDialog(PTDialog);
	int Input(PTWindowObject, LPSTR, int, LPCSTR, ...);
	int FileOpen(PTWindowObject, RFileDlgData);
	int FileSave(PTWindowObject, RFileDlgData);
	virtual void Idle();
	int MsgBox(PTWindowObject, UINT, LPCSTR, LPCSTR, ...);
	virtual BOOL ProcessMsgs(MSG _FAR &);
	virtual BOOL ProcessDlgMsgs(MSG _FAR &);
	virtual BOOL ProcessAccels(MSG _FAR &);
	virtual BOOL ProcessMDIAccels(MSG _FAR &);
};

enum ToolBarBtnType { NORMAL, THREESTATE };

_CLASSDEF(TToolBarInfo)
struct TToolBarInfo
{
	int nID;
	ToolBarBtnType nType;
	BOOL bDown;
	HBITMAP hBmp[4];
	BOOL bReserved;
};

class _CLASSTYPE TToolBar : public TWindow
{
	BOOL bDone;
    HBITMAP hPatBmp;

protected:
	int nNumButtons;
	int nHeight, nWidth;

	HBITMAP DuplicateBitmap(HBITMAP);
	void DrawBitmap(HDC, HBITMAP, DWORD);

public:
	int nMaxWidth;
	PTToolBarInfo pInfo;

	TToolBar(PTWindowObject, LPCSTR, PTToolBarInfo, int);
	~TToolBar();
	void AddButton(PTToolBarInfo,int=0);
	void RemoveButton(int);
	void SetState(int, BOOL);
	int GetPos(int);    
    void SetType(int, ToolBarBtnType);
    void SetBitmap(int, int, HBITMAP);
	virtual PTPushButton ConstructButton(int);
	virtual void CalcWindowSize(int, int, int&, int&);
	virtual void WMSize(RTMessage) = [WM_FIRST + WM_SIZE];
	virtual void WMDrawItem(RTMessage) = [WM_FIRST + WM_DRAWITEM];
	virtual void WMGetMinMaxInfo(RTMessage) = [WM_FIRST + WM_GETMINMAXINFO];
    virtual void WMCommand(RTMessage) = [WM_FIRST + WM_COMMAND];
};

//Global functions
typedef void (_FAR *MRFUNCTION)(PTWindowObject, RTMessage);

//the following function is built into the BORLANDC libraries, but is not
//prototyped anywhere.  Its parameters were determined from TLIB and looking
//at compiled code for return value.
MRFUNCTION _DDVTdispatchNULL(LPVOID, int);

LONG _FAR UMP(RTMessage);
void _export _FAR SetGlobalWindowObject(PTWindowObject);

#endif	//__WOLF_H

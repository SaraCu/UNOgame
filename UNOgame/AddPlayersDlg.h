#pragma once

#include "CPlayer.h"

// CAddPlayersDlg dialog

class CAddPlayersDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPlayersDlg)

public:

	CAddPlayersDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddPlayersDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_PLAYERS_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual INT_PTR DoModal();

	afx_msg void OnBnClickedAddChange();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnEnChangeNameEdit();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	CButton AddChangeButton;
	CEdit NameEditBox;
	CListBox PlayersListBox;
	CButton ClearButton;
	CButton DeleteButton;
	CButton RenameButton;
	CButton UpButton;
	CButton DownButton;

protected:
	bool IsChangeMode = false;
	int ChangeModeIndex = 0;
	void SetControlsState();
	bool DoesNameExist(CString name);
	PlayerList* pPlayers = NULL;

public:
	afx_msg void OnLbnSelchangeListPlayers();
	afx_msg void OnBnClickedRename();
	afx_msg void OnBnClickedClear();

	void DeletePlayers();
	PlayerList* GetPlayers() {return pPlayers;}
	void SetPlayers(PlayerList* pplayers) { pPlayers = pplayers; }
	CButton okButton;
};

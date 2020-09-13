// AddPlayersDlg.cpp : implementation file
//

#include "pch.h"
#include "UNOgame.h"
#include "AddPlayersDlg.h"
#include "afxdialogex.h"
#include "InfoDlg.h"

// CAddPlayersDlg dialog

IMPLEMENT_DYNAMIC(CAddPlayersDlg, CDialogEx)

CAddPlayersDlg::CAddPlayersDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_PLAYERS_DIALOG, pParent)
{
}

CAddPlayersDlg::~CAddPlayersDlg()
{
}

void CAddPlayersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_CHANGE, AddChangeButton);
	DDX_Control(pDX, IDC_NAME_EDIT, NameEditBox);
	DDX_Control(pDX, IDC_LIST_PLAYERS, PlayersListBox);

	DDX_Control(pDX, IDC_CLEAR, ClearButton);
	DDX_Control(pDX, IDC_DELETE, DeleteButton);
	DDX_Control(pDX, IDC_RENAME, RenameButton);
	DDX_Control(pDX, IDC_UP, UpButton);
	DDX_Control(pDX, IDC_DOWN, DownButton);
	DDX_Control(pDX, IDOK, okButton);
}

BEGIN_MESSAGE_MAP(CAddPlayersDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_CHANGE, &CAddPlayersDlg::OnBnClickedAddChange)
	ON_BN_CLICKED(IDC_DELETE, &CAddPlayersDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_UP, &CAddPlayersDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_DOWN, &CAddPlayersDlg::OnBnClickedDown)
	ON_EN_CHANGE(IDC_NAME_EDIT, &CAddPlayersDlg::OnEnChangeNameEdit)
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_LIST_PLAYERS, &CAddPlayersDlg::OnLbnSelchangeListPlayers)
	ON_BN_CLICKED(IDC_RENAME, &CAddPlayersDlg::OnBnClickedRename)
	ON_BN_CLICKED(IDC_CLEAR, &CAddPlayersDlg::OnBnClickedClear)
END_MESSAGE_MAP()

// CAddPlayersDlg message handlers
bool CAddPlayersDlg::DoesNameExist(CString name)
{
	if (pPlayers->GetCount() == 0) return false;

	CPlayer* player;
	POSITION pos = pPlayers->GetHeadPosition();
	for (int i = 0; i < pPlayers->GetCount(); i++)
	{
		player = pPlayers->GetNext(pos);
		if (player->GetName() == name) return true;
	}
	return false;
}

void CAddPlayersDlg::OnBnClickedAddChange()
{
	CString name;
	NameEditBox.GetWindowText(name);

	if (DoesNameExist(name))
	{
		CInfoDlg dlgInfo;
		dlgInfo.SetInfo(_T("Info"), _T("Player name already exists on the list!"));
		dlgInfo.DoModal();
		return;
	}

	if (!IsChangeMode)
	{
		CPlayer* pNewPlayer = new CPlayer();
		pNewPlayer->SetName(name);
		pPlayers->AddTail(pNewPlayer);

		PlayersListBox.AddString(name);

		NameEditBox.SetWindowTextW(_T(""));

		GotoDlgCtrl(&NameEditBox);
	}
	else
	{
		IsChangeMode = false;

		CPlayer* pPlayer = pPlayers->GetAt(pPlayers->FindIndex(ChangeModeIndex));
		pPlayer->SetName(name);

		PlayersListBox.SetRedraw(FALSE);
		PlayersListBox.DeleteString(ChangeModeIndex);
		PlayersListBox.InsertString(ChangeModeIndex, name);
		PlayersListBox.SetRedraw(TRUE);
		PlayersListBox.UpdateWindow();

		NameEditBox.SetWindowTextW(_T(""));

		GotoDlgCtrl(&NameEditBox);
	}

	if (pPlayers->GetCount() == 10)
	{
		AddChangeButton.EnableWindow(FALSE);
	}

	SetControlsState();
}

void CAddPlayersDlg::OnBnClickedDelete()
{
	int idx = PlayersListBox.GetCurSel();

	CPlayer* pPlayer = pPlayers->GetAt(pPlayers->FindIndex(idx));
	pPlayers->RemoveAt(pPlayers->FindIndex(idx));
	delete pPlayer;

	PlayersListBox.SetRedraw(FALSE);
	PlayersListBox.DeleteString(idx);
	PlayersListBox.SetRedraw(TRUE);
	PlayersListBox.UpdateWindow();

	SetControlsState();
}

void CAddPlayersDlg::OnBnClickedUp()
{
	int idx = PlayersListBox.GetCurSel();
	if (idx == 0) return;

	POSITION posPrev = pPlayers->FindIndex(idx - 1);
	CPlayer* pPlayer = pPlayers->GetAt(pPlayers->FindIndex(idx));
	pPlayers->RemoveAt(pPlayers->FindIndex(idx));
	pPlayers->InsertBefore(posPrev, pPlayer);

	PlayersListBox.SetRedraw(FALSE);
	PlayersListBox.DeleteString(idx);
	PlayersListBox.InsertString(idx - 1, pPlayer->GetName());
	PlayersListBox.SetCurSel(idx - 1);
	PlayersListBox.SetRedraw(TRUE);
	PlayersListBox.UpdateWindow();

	SetControlsState();
}

void CAddPlayersDlg::OnBnClickedDown()
{
	int idx = PlayersListBox.GetCurSel();
 	if (idx == PlayersListBox.GetCount() - 1) return;

	CPlayer* pPlayer = pPlayers->GetAt(pPlayers->FindIndex(idx));
	pPlayers->RemoveAt(pPlayers->FindIndex(idx));
	POSITION posNext = pPlayers->FindIndex(idx);
	pPlayers->InsertAfter(posNext, pPlayer);

	PlayersListBox.SetRedraw(FALSE);
	PlayersListBox.DeleteString(idx);
	PlayersListBox.InsertString(idx + 1, pPlayer->GetName());
	PlayersListBox.SetCurSel(idx + 1);
	PlayersListBox.SetRedraw(TRUE);
	PlayersListBox.UpdateWindow();

	SetControlsState();
}

INT_PTR CAddPlayersDlg::DoModal()
{
	if (pPlayers == NULL)
	{
		pPlayers = new PlayerList();
	}

	return CDialogEx::DoModal();
}

void CAddPlayersDlg::OnEnChangeNameEdit()
{
	CString text;
	NameEditBox.GetWindowText(text);

	if (text.GetLength() == 0)
	{
		AddChangeButton.EnableWindow(FALSE);
	}
	else
	{
		AddChangeButton.EnableWindow(TRUE);
	}
}

void CAddPlayersDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	NameEditBox.SetLimitText(10);

	SetControlsState();
}

void CAddPlayersDlg::SetControlsState()
{
	BOOL maxCount = PlayersListBox.GetCount() == 10;
	int i = PlayersListBox.GetCurSel();
	BOOL slected = i!=-1;

	if (!IsChangeMode)
	{
		PlayersListBox.EnableWindow(TRUE);
		AddChangeButton.SetWindowTextW(_T("Add"));
		AddChangeButton.EnableWindow(!maxCount);
		ClearButton.ShowWindow(SW_HIDE);
		DeleteButton.EnableWindow(slected);
		RenameButton.EnableWindow(slected);
		UpButton.EnableWindow(slected);
		DownButton.EnableWindow(slected);
	}
	else
	{
		PlayersListBox.EnableWindow(FALSE);
		AddChangeButton.SetWindowTextW(_T("Change"));
		AddChangeButton.EnableWindow(TRUE);
		ClearButton.ShowWindow(SW_SHOW);
		DeleteButton.EnableWindow(FALSE);
		RenameButton.EnableWindow(FALSE);
		UpButton.EnableWindow(FALSE);
		DownButton.EnableWindow(FALSE);
	}

	CString text;
	NameEditBox.GetWindowText(text);

	if (text.GetLength() == 0)
	{
		AddChangeButton.EnableWindow(FALSE);
	}
	else
	{
		AddChangeButton.EnableWindow(TRUE);
	}

	if (PlayersListBox.GetCount() > 1)
	{
		okButton.EnableWindow(TRUE);
	}
	else
	{
		okButton.EnableWindow(FALSE);
	}
}

void CAddPlayersDlg::OnLbnSelchangeListPlayers()
{
	SetControlsState();
}

void CAddPlayersDlg::OnBnClickedRename()
{
	IsChangeMode = true;

	ChangeModeIndex = PlayersListBox.GetCurSel();

	CPlayer* pPlayer = pPlayers->GetAt(pPlayers->FindIndex(ChangeModeIndex));
	NameEditBox.SetWindowTextW(pPlayer->GetName());

	SetControlsState();

	GotoDlgCtrl(&NameEditBox);
}

void CAddPlayersDlg::OnBnClickedClear()
{
	IsChangeMode = false;

	NameEditBox.SetWindowTextW(_T(""));
	SetControlsState();

	GotoDlgCtrl(&NameEditBox);
}

void CAddPlayersDlg::DeletePlayers()
{
	if (pPlayers->GetCount() > 0)
	{
		CPlayer* player;
		POSITION pos = pPlayers->GetHeadPosition();
		for (int i = 0; i < pPlayers->GetCount(); i++)
		{
			player = pPlayers->GetNext(pos);
			CString name = player->GetName();
			delete player;
		}

		pPlayers->RemoveAll();
	}

	delete pPlayers;
	pPlayers = NULL;
}
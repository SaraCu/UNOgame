// ChooseDlg.cpp : implementation file
//

#include "pch.h"
#include "UNOgame.h"
#include "ChooseDlg.h"
#include "afxdialogex.h"

// CChooseDlg dialog

IMPLEMENT_DYNAMIC(CChooseDlg, CDialogEx)

CChooseDlg::CChooseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHOOSE_DIALOG, pParent)
{
}

CChooseDlg::~CChooseDlg()
{
}

void CChooseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, OKButton);
	DDX_Control(pDX, IDCANCEL, CancelButton);
	DDX_Control(pDX, IDC_TEXT_STATIC, InfoStatic);
}

BEGIN_MESSAGE_MAP(CChooseDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChooseDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChooseDlg::OnBnClickedCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CChooseDlg message handlers
void CChooseDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CChooseDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

HBRUSH CChooseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(0, 0, 0));
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

BOOL CChooseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetBackgroundColor(RGB(0, 0, 0));

	OKButton.SetWindowTextW(OkButtonText);
	CancelButton.SetWindowTextW(OkCancelButtonText);
	InfoStatic.SetWindowTextW(Text);

	return TRUE;  // return TRUE unless you set the focus to a control
}

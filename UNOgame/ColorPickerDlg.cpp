// ColorPickerDlg.cpp : implementation file
//

#include "pch.h"
#include "UNOgame.h"
#include "ColorPickerDlg.h"
#include "afxdialogex.h"


// CColorPickerDlg dialog

IMPLEMENT_DYNAMIC(CColorPickerDlg, CDialogEx)

CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SET_COLOR_DIALOG, pParent)
{

}

CColorPickerDlg::~CColorPickerDlg()
{
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_STATIC, NameStatic);
}


BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialogEx)
	ON_STN_CLICKED(IDC_RED_STATIC, &CColorPickerDlg::OnClickedRedStatic)
	ON_STN_CLICKED(IDC_YELLOW_STATIC, &CColorPickerDlg::OnClickedYellowStatic)
	ON_STN_CLICKED(IDC_GREEN_STATIC, &CColorPickerDlg::OnClickedGreenStatic)
	ON_STN_CLICKED(IDC_BLUE_STATIC, &CColorPickerDlg::OnClickedBlueStatic)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CColorPickerDlg message handlers


BOOL CColorPickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetBackgroundColor(RGB(0, 0, 0));

	CString val = _T("Player  ")+ Name;
	val += _T("  must choose a color:");
	NameStatic.SetWindowTextW(val);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CColorPickerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CColorPickerDlg::OnClickedRedStatic()
{
	Color = enColor::Red;
	this->EndDialog(0);
}


void CColorPickerDlg::OnClickedYellowStatic()
{
	Color = enColor::Yellow;
	this->EndDialog(0);
}


void CColorPickerDlg::OnClickedGreenStatic()
{
	Color = enColor::Green;
	this->EndDialog(0);
}


void CColorPickerDlg::OnClickedBlueStatic()
{
	Color = enColor::Blue;
	this->EndDialog(0);
}

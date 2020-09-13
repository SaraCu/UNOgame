// InfoDlg.cpp : implementation file
//

#include "pch.h"
#include "UNOgame.h"
#include "InfoDlg.h"
#include "afxdialogex.h"

// CInfoDlg dialog

IMPLEMENT_DYNAMIC(CInfoDlg, CDialogEx)

CInfoDlg::CInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INFO_DIALOG, pParent)
{
}

CInfoDlg::~CInfoDlg()
{
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO_TEXT, InfoTextLabel);
	DDX_Control(pDX, IDOK, OKButton);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CInfoDlg message handlers

void CInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
}

HBRUSH CInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(BackColor);
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}


BOOL CInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextW(Title);
	InfoTextLabel.SetWindowTextW(Info);

	if (RoundWinner || Winner)
	{
		BackColor = RGB(128, 0, 0);
		this->SetBackgroundColor(BackColor);

		if (Winner)
		{
			this->MoveWindow(0, 0, 800, 600);
			this->CenterWindow(NULL);
			InfoTextLabel.CenterWindow(NULL);
			OKButton.CenterWindow(NULL);
			CRect client;
			GetClientRect(client);
			CRect rct;
			OKButton.GetWindowRect(rct);
			OKButton.MoveWindow(rct.left, client.bottom - 30, rct.Width(), rct.Height());
			InfoTextLabel.GetWindowRect(rct);
			OKButton.MoveWindow(client.Width() / 2 - (100), client.bottom - 100, 200, rct.Height());
			bmpStart = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_START_BITMAP));
		}
	}
	else
	{
		BackColor = RGB(0, 0, 0);
		this->SetBackgroundColor(BackColor);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (Winner)
	{
		Gdiplus::Graphics DialogGraphics(dc);
		DialogGraphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);

		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);
		Gdiplus::Rect rctDest = Gdiplus::Rect(ClinetRect.left, ClinetRect.top, ClinetRect.Width(), ClinetRect.Height());

		Gdiplus::Rect sourceRct;

		CSize imgSize(bmpStart->GetWidth(), bmpStart->GetHeight());

		bool fitWitdh = false;

		double propSrc = ((double)imgSize.cx) / ((double)imgSize.cy);
		double propDst = ((double)ClinetRect.Width()) / ((double)ClinetRect.Height());
		fitWitdh = propSrc < propDst;

		if (fitWitdh)
		{
			sourceRct.Width = imgSize.cx;
			sourceRct.Height = imgSize.cx * ClinetRect.Height() / ClinetRect.Width() + 1;
			if (sourceRct.Height > imgSize.cy) sourceRct.Height = imgSize.cy;
			sourceRct.X = 0;
			sourceRct.Y = (imgSize.cy - sourceRct.Height) / 2;
		}
		else
		{
			sourceRct.Height = imgSize.cy;
			sourceRct.Width = imgSize.cy * ClinetRect.Width() / ClinetRect.Height() + 1;
			if (sourceRct.Width > imgSize.cx) sourceRct.Width = imgSize.cx;
			sourceRct.X = (imgSize.cx - sourceRct.Width) / 2;
			sourceRct.Y = 0;
		}

		DialogGraphics.DrawImage(bmpStart, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
	}
}


void CInfoDlg::OnClose()
{	
	if (bmpStart != NULL) delete bmpStart;

	CDialogEx::OnClose();
}

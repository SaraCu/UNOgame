#pragma once


// CInfoDlg dialog

class CInfoDlg : public CDialogEx
{
	CString Title; 
	CString Info;
	bool Winner = false;
	bool RoundWinner = false;
	COLORREF BackColor;
	Gdiplus::Bitmap* bmpStart = NULL;

	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CInfoDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INFO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CStatic InfoTextLabel;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

public:
	void SetInfo(CString title, CString info)
	{
		Title = title;
		Info = info;
	}

	void SetWinnerFlag(bool winner) { Winner = winner; }
	void SetRoundWinnerFlag(bool roundWinner) { RoundWinner = roundWinner; }
	CButton OKButton;
	afx_msg void OnPaint();
	afx_msg void OnClose();
};

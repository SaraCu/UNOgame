#pragma once

// CChooseDlg dialog

class CChooseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChooseDlg)
	CString OkButtonText;
	CString OkCancelButtonText;
	CString Text;

public:
	CChooseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CChooseDlg();

	void SetOkButton(CString text)
	{
		OkButtonText = text;
	}

	void SetCancelButton(CString text)
	{
		OkCancelButtonText = text;
	}

	void SetText(CString text)
	{
		Text = text;
	}

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOOSE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	CButton OKButton;
	CButton CancelButton;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic InfoStatic;
};

#pragma once
#include "CCard.h"

// CColorPickerDlg dialog

class CColorPickerDlg : public CDialogEx
{
	CString Name;
	enColor Color = enColor::Any;
	DECLARE_DYNAMIC(CColorPickerDlg)
public:
	CColorPickerDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CColorPickerDlg();
	void SetPlayerName(CString name) { Name = name; }
	enColor GetColor() { return Color; }
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_COLOR_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedRedStatic();
	afx_msg void OnClickedYellowStatic();
	afx_msg void OnClickedGreenStatic();
	afx_msg void OnClickedBlueStatic();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic NameStatic;
};

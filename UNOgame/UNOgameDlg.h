
// UNOgameDlg.h : header file
//

#pragma once
#include "gdiplus.h"
#include "CCard.h"
#include "CDeck.h"
#include "CPlayer.h"
#include "CUNOGameFlow.h"

// CUNOgameDlg dialog
class CUNOgameDlg : public CDialogEx
{
// Construction
public:
	CUNOgameDlg(CWnd* pParent = nullptr);	// standard constructor
	void RefreshDialog(BOOL earse);
	void UpdatePlayerList();
	Gdiplus::Rect SourceCardRect(CCard* card, enColor currentColor);
	void DrawSelectedCard(Gdiplus::Graphics* graphics, CCard* card);
	void DrawPileCard(Gdiplus::Graphics* graphics, CCard* card, enColor currentColor);
	void DrawPlayerDeck(Gdiplus::Graphics* graphics);
	void DrawStart(Gdiplus::Graphics* graphics);
	void AdjustContolsLayout();
	void VisibleDeckCardPositions( CSize& cardDestSize, int& startX, int& stepX, int& startY);
	CCard* LastHoverCard = NULL;
	CCard* HoverCard = NULL;
	int HoverCardIndex = -1;
	CUNOGameFlow UNOGameFlow;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNOGAME_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	Gdiplus::Bitmap* bmpCards = NULL;
	Gdiplus::Bitmap* bmpStart = NULL;
	Gdiplus::Bitmap* bmpStartDark = NULL;
	const Gdiplus::Rect CardDimension = Gdiplus::Rect(0, 0, 256, 383);
	bool FullRedraw = false;
	bool Throwing = false;
	
	

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddPlayers();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	CButton AddPlayersButton;
	CButton UNONotCalledButton;
	CButton NextButton;
	CListBox PlayersListBox;
	CButton DrawButton;
	afx_msg void OnBnClickedDrawButton();
	afx_msg void OnBnClickedNextButton();
	afx_msg void OnBnClickedUnoNotCalledButton();
	CStatic CardsGroup;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic StaticPlayerText;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLbnSelchangePlayerList();
};

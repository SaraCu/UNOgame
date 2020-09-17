
// UNOgameDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "UNOgame.h"
#include "UNOgameDlg.h"
#include "afxdialogex.h"
#include "AddPlayersDlg.h"
#include "CPlayer.h"
#include "CDeck.h"
#include "ColorPickerDlg.h"
#include "InfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUNOgameDlg dialog



CUNOgameDlg::CUNOgameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_UNOGAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUNOgameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_PLAYERS, AddPlayersButton);
	DDX_Control(pDX, IDC_UNO_NOT_CALLED_BUTTON, UNONotCalledButton);
	DDX_Control(pDX, IDC_NEXT_BUTTON, NextButton);
	DDX_Control(pDX, IDC_PLAYER_LIST, PlayersListBox);
	DDX_Control(pDX, IDC_DRAW_BUTTON, DrawButton);
	DDX_Control(pDX, IDC_CARDS_GROUP, CardsGroup);
	DDX_Control(pDX, IDC_STATIC_PLAYER, StaticPlayerText);
}

BEGIN_MESSAGE_MAP(CUNOgameDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ADD_PLAYERS, &CUNOgameDlg::OnBnClickedAddPlayers)
	ON_WM_SIZING()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_DRAW_BUTTON, &CUNOgameDlg::OnBnClickedDrawButton)
	ON_BN_CLICKED(IDC_NEXT_BUTTON, &CUNOgameDlg::OnBnClickedNextButton)
	ON_BN_CLICKED(IDC_UNO_NOT_CALLED_BUTTON, &CUNOgameDlg::OnBnClickedUnoNotCalledButton)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_LBN_SELCHANGE(IDC_PLAYER_LIST, &CUNOgameDlg::OnLbnSelchangePlayerList)
END_MESSAGE_MAP()


HBRUSH CUNOgameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
// CUNOgameDlg message handlers

BOOL CUNOgameDlg::OnInitDialog()
{
	bmpCards = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_UNO_CARDS_BITMAP));
	bmpStart = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_START_BITMAP));
	bmpStartDark = Gdiplus::Bitmap::FromResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_START_DARK_BITMAP));

	CDialogEx::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	AdjustContolsLayout();

	UNOGameFlow.SetMainDilog(this);

	this->SetBackgroundColor(RGB(0, 0, 0));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUNOgameDlg::OnClose()
{
	if (bmpCards != NULL) delete bmpCards;
	if (bmpStart != NULL) delete bmpStart;
	if (bmpStartDark != NULL) delete bmpStartDark;

	CDialogEx::OnClose();
}


void CUNOgameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CUNOgameDlg::AdjustContolsLayout()
{
	if (AddPlayersButton.m_hWnd == NULL) return;

	CRect ClientRect;
	this->GetClientRect(&ClientRect);
	CRect ButtonRect, PlayersListBoxRect, StaticPlayerRect;
	DrawButton.GetClientRect(&ButtonRect);
	PlayersListBox.GetClientRect(&PlayersListBoxRect);
	
	PlayersListBox.MoveWindow(10, 0, 300, 140);

	DrawButton.MoveWindow((
		ClientRect.Width() - ButtonRect.Width()) / 2 - 5 - 25,
		(ClientRect.Height() / 2) + (ClientRect.Height() / 4) - 103 - ButtonRect.Height(),
		ButtonRect.Width(),
		ButtonRect.Height());

	NextButton.MoveWindow((
		ClientRect.Width() + ButtonRect.Width()) / 2 + 5 - 25, 
		(ClientRect.Height() / 2) + (ClientRect.Height() / 4) - 103 - ButtonRect.Height(),
		ButtonRect.Width(),
		ButtonRect.Height());

	UNONotCalledButton.MoveWindow(
		180,
		(ClientRect.Height() / 2) + (ClientRect.Height() / 4) - 103 - ButtonRect.Height(),
		ButtonRect.Width(),
		ButtonRect.Height());

	CardsGroup.MoveWindow(10,
		(ClientRect.Height() / 2) + (ClientRect.Height() / 4) - 110,
		ClientRect.Width() - 20,
		220);

	StaticPlayerText.MoveWindow(
		10,
		(ClientRect.Height() / 2) + (ClientRect.Height() / 4) - 120,
		220,
		ButtonRect.Height());

	CPlayer* player = UNOGameFlow.GetPlayerWithVisibleCards();
	if (player != NULL)
	{
		CString out = player->GetName();
		out = _T(" ") + out + CString(MAKEINTRESOURCE(IDS_S_CARDS));
		StaticPlayerText.SetWindowTextW(out);
	}
	else 
	{
		StaticPlayerText.SetWindowTextW(_T(""));
	}

	if (!UNOGameFlow.IsGameStarted())
	{
		AddPlayersButton.ShowWindow(SW_SHOW);

		DrawButton.ShowWindow(SW_HIDE);
		UNONotCalledButton.ShowWindow(SW_HIDE);
		NextButton.ShowWindow(SW_HIDE);
		PlayersListBox.ShowWindow(SW_HIDE);
		CardsGroup.ShowWindow(SW_HIDE);
		StaticPlayerText.ShowWindow(SW_HIDE);
	}
	else
	{
		AddPlayersButton.ShowWindow(SW_HIDE);
		CardsGroup.ShowWindow(SW_SHOW);

		if (UNOGameFlow.GetPlayerWithVisibleCards() != NULL)
		{
			StaticPlayerText.ShowWindow(SW_SHOW);
		}
		else
		{
			StaticPlayerText.ShowWindow(SW_HIDE);
		}

		if (!UNOGameFlow.IsRoundStarted())
		{
			DrawButton.ShowWindow(SW_HIDE);
			UNONotCalledButton.ShowWindow(SW_HIDE);
			NextButton.ShowWindow(SW_HIDE);
			PlayersListBox.ShowWindow(SW_SHOW);
		}
		else
		{
			DrawButton.ShowWindow(SW_SHOW);
			UNONotCalledButton.ShowWindow(SW_SHOW);
			NextButton.ShowWindow(SW_SHOW);
			PlayersListBox.ShowWindow(SW_SHOW);

			if (UNOGameFlow.CanThrowAnyCard())
			{
				DrawButton.EnableWindow(FALSE);
			}
			else
			{
				if (NextButton.IsWindowEnabled())
				{
					DrawButton.EnableWindow(FALSE);
				}
				else 
				{
					DrawButton.EnableWindow(TRUE);
				}
			}
		}
	}


}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUNOgameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC DialogDC(this);
		Gdiplus::Graphics DialogGraphics(DialogDC.m_hDC);
		DialogGraphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);

		if (UNOGameFlow.IsGameStarted())
		{
			DrawStart(&DialogGraphics);

			if (LastHoverCard == NULL || HoverCard == NULL || FullRedraw || Throwing)
			{
				FullRedraw = false;

				CCard* card = UNOGameFlow.GetDiscardCard();
				enColor color = UNOGameFlow.GetDiscardColor();

				DrawPileCard(&DialogGraphics, card, color);
			}

			if (HoverCard != NULL && !Throwing) DrawSelectedCard(&DialogGraphics, HoverCard);

			if (UNOGameFlow.GetPlayerWithVisibleCards() != NULL) DrawPlayerDeck(&DialogGraphics);

			
		}
		else
		{
			DrawStart(&DialogGraphics);
		}
		

		CDialogEx::OnPaint();
	}
}
void CUNOgameDlg::DrawSelectedCard(Gdiplus::Graphics* graphics, CCard* card)
{
	CRect ClinetRect;
	this->GetClientRect(&ClinetRect);

	int destCardHeight = ClinetRect.Height() / 2 - 40;

	CSize cardDestSize(CardDimension.Width * destCardHeight / CardDimension.Height, destCardHeight);

	Gdiplus::Rect rctDest = Gdiplus::Rect(
		ClinetRect.left + (ClinetRect.Width() / 2) + (ClinetRect.Width() / 4) - (cardDestSize.cx / 2) + (cardDestSize.cx / 2),
		ClinetRect.top + (ClinetRect.Height() / 4) - (cardDestSize.cy / 2),
		cardDestSize.cx,
		cardDestSize.cy);

	Gdiplus::Rect sourceRct = SourceCardRect(card, card->GetColor());

	graphics->DrawImage(bmpCards, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
}

void CUNOgameDlg::DrawPileCard(Gdiplus::Graphics* graphics, CCard* card, enColor currentColor)
{
	CRect ClinetRect;
	this->GetClientRect(&ClinetRect);

	int destCardHeight = ClinetRect.Height() / 2 - 40;

	CSize cardDestSize(CardDimension.Width * destCardHeight/ CardDimension.Height, destCardHeight);

	Gdiplus::Rect rctDest = Gdiplus::Rect(
		ClinetRect.left + (ClinetRect.Width()/2) + (ClinetRect.Width() / 4) - (cardDestSize.cx / 2),
		ClinetRect.top + (ClinetRect.Height() / 4) - (cardDestSize.cy / 2),
		cardDestSize.cx,
		cardDestSize.cy);

	Gdiplus::Rect sourceRct = SourceCardRect(card, currentColor);

	graphics->DrawImage(bmpCards, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
}
void CUNOgameDlg::DrawPlayerDeck(Gdiplus::Graphics* graphics)
{
	CSize cardDestSize;
	int startX;
	int startY;
	int stepX;

	VisibleDeckCardPositions(cardDestSize, startX, stepX, startY);

	CPlayer* visiblePlayer = UNOGameFlow.GetPlayerWithVisibleCards();

	CDeck* visibleDeck = visiblePlayer->Deck();

	POSITION pos = visibleDeck->Cards.GetHeadPosition();
	for (int i = 0; i < visibleDeck->GetCardsCount(); i++)
	{
		Gdiplus::Rect rctDest = Gdiplus::Rect(
			startX,
			startY,
			cardDestSize.cx,
			cardDestSize.cy);
		
		startX += stepX;

		CCard* card = visibleDeck->Cards.GetNext(pos);

		Gdiplus::Rect sourceRct = SourceCardRect(card, card->GetColor());

		graphics->DrawImage(bmpCards, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
	}
}

void CUNOgameDlg::VisibleDeckCardPositions(CSize& cardDestSize, int& startX, int& stepX, int& startY)
{
	CRect ClinetRect;
	this->GetClientRect(&ClinetRect);

	CDeck* visibleDeck = UNOGameFlow.GetPlayerWithVisibleCards()->Deck();

	cardDestSize = CSize(CardDimension.Width / 2, CardDimension.Height / 2);

	startY = ClinetRect.top + (ClinetRect.Height() / 2) + (ClinetRect.Height() / 4) - (cardDestSize.cy / 2);

	int count = visibleDeck->GetCardsCount();

	int marginX = 20;

	int spaceW = (ClinetRect.Width() - (marginX * 2));

	if (spaceW >= count * cardDestSize.cx)
	{
		startX = (ClinetRect.Width() / 2) - (count * cardDestSize.cx / 2) + marginX;
		stepX = cardDestSize.cx;
	}
	else
	{
		startX = marginX;
		stepX = (spaceW - cardDestSize.cx) / (count - 1);
	}
}

Gdiplus::Rect CUNOgameDlg::SourceCardRect(CCard* card, enColor targetColor)
{
	int X;
	int Y;
	if (targetColor == enColor::Any)
	{
		if (card->GetType() == enCardType::DrawFour || card->GetType() == enCardType::Wilde)
		{
			X = CardDimension.Width * 15;
			if (card->GetType() == enCardType::DrawFour)
				Y = CardDimension.Height * 1;
			else
				Y = CardDimension.Height * 0;
		}
		else
		{
			X = CardDimension.Width * (int)card->GetType();
			Y = CardDimension.Height * (int)card->GetColor();
		}
	}
	else
	{
		X = CardDimension.Width * (int)card->GetType();
		Y = CardDimension.Height * (int)targetColor;
	}
	return Gdiplus::Rect(X, Y, CardDimension.Width, CardDimension.Height);
}

void CUNOgameDlg::DrawStart(Gdiplus::Graphics* graphics)
{
	CRect ClientRect;
	this->GetClientRect(&ClientRect);
	Gdiplus::Rect rctDest = Gdiplus::Rect(ClientRect.left, ClientRect.top, ClientRect.Width(), ClientRect.Height());

	Gdiplus::Rect sourceRct;

	CSize imgSize(bmpStart->GetWidth(), bmpStart->GetHeight());

	bool fitWitdh = false;

	double propSrc = ((double)imgSize.cx) / ((double)imgSize.cy);
	double propDst = ((double)ClientRect.Width()) / ((double)ClientRect.Height());
	fitWitdh = propSrc < propDst;

	if (fitWitdh)
	{
		sourceRct.Width = imgSize.cx;
		sourceRct.Height = imgSize.cx * ClientRect.Height() / ClientRect.Width() + 1;
		if (sourceRct.Height > imgSize.cy) sourceRct.Height = imgSize.cy;
		sourceRct.X = 0;
		sourceRct.Y = (imgSize.cy - sourceRct.Height) / 2;
	}
	else
	{
		sourceRct.Height = imgSize.cy;
		sourceRct.Width = imgSize.cy * ClientRect.Width() / ClientRect.Height() + 1;
		if (sourceRct.Width > imgSize.cx) sourceRct.Width = imgSize.cx;
		sourceRct.X = (imgSize.cx - sourceRct.Width) / 2;
		sourceRct.Y = 0;
	}

	if (!UNOGameFlow.IsGameStarted())
	{
		graphics->DrawImage(bmpStart, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
	}
	else
	{
		graphics->DrawImage(bmpStartDark, rctDest, sourceRct.GetLeft(), sourceRct.GetTop(), sourceRct.Width, sourceRct.Height, Gdiplus::Unit::UnitPixel);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUNOgameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define MINW 800
#define MINH 500

void CUNOgameDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CDialog::OnGetMinMaxInfo(lpMMI);

	if (lpMMI->ptMinTrackSize.x < MINW)
		lpMMI->ptMinTrackSize.x = MINW;
	if (lpMMI->ptMinTrackSize.y < MINH)
		lpMMI->ptMinTrackSize.y = MINH;
}

void CUNOgameDlg::RefreshDialog(BOOL earse)
{
	FullRedraw = true;

	CRect ClinetRect;
	this->GetClientRect(&ClinetRect);
	InvalidateRect(ClinetRect, earse);

	AdjustContolsLayout();
}

void CUNOgameDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	static CRect lastRect = CRect(0, 0, 0, 0);

	CDialogEx::OnSizing(fwSide, pRect);

	CRect newRect = pRect;



	if (lastRect.Width() != newRect.Width() || lastRect.Height() != newRect.Height())
	{
		bool earse = UNOGameFlow.IsGameStarted();

		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);
		InvalidateRect(ClinetRect, earse);

		lastRect = pRect;
	}

	AdjustContolsLayout();
}
void CUNOgameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (SIZE_MAXIMIZED == nType || SIZE_RESTORED == nType)
	{
		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);
		InvalidateRect(ClinetRect);

		AdjustContolsLayout();
	}
}



void CUNOgameDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	bool Invalidate = false;

	CDeck* visibleDeck = NULL;
	if (UNOGameFlow.GetPlayerWithVisibleCards() != NULL)
	{
		visibleDeck = UNOGameFlow.GetPlayerWithVisibleCards()->Deck();
	};

	if (visibleDeck != NULL)
	{
		CSize cardDestSize;
		int startX;
		int startY;
		int stepX;

		VisibleDeckCardPositions(cardDestSize, startX, stepX, startY);

		if (point.y >= startY &&
			point.y <= startY + cardDestSize.cy
			)
		{
			HoverCardIndex = -1;
			int count = visibleDeck->GetCardsCount();
			if (stepX != cardDestSize.cx)
			{
				if (point.x >= startX && point.x <= (startX + ((count - 1) * stepX) + cardDestSize.cx))
				{
					HoverCardIndex = (point.x - startX) / stepX;
					if (HoverCardIndex >= count)
					{
						HoverCardIndex = count - 1;
					}
				}
			}
			else
			{
				if (point.x >= startX && point.x <= count * cardDestSize.cx + startX)
				{
					HoverCardIndex = (point.x - startX) / stepX;
					if (HoverCardIndex >= count)
					{
						HoverCardIndex = count - 1;
					}
				}
			}
			if (HoverCardIndex == -1)
			{
				HoverCard = NULL;
				if (LastHoverCard != NULL)
				{
					LastHoverCard = NULL;
					Invalidate = true;
				}
			}
			else
			{
				HoverCard = visibleDeck->GetCardAt(HoverCardIndex);
				if (LastHoverCard != HoverCard)
				{
					Invalidate = true;
					LastHoverCard = HoverCard;
				}
			}
		}
		else
		{
			HoverCard = NULL;
			if (LastHoverCard != NULL)
			{
				LastHoverCard = NULL;
				Invalidate = true;
			}
		}
	}

	if (Invalidate)
	{
		CRect ClinetRect;
		this->GetClientRect(&ClinetRect);
		CRect InvalidRect(
			(int)((ClinetRect.Width() / 2) + (ClinetRect.Width() / 4)),
			0,
			ClinetRect.Width(),
			ClinetRect.Height() / 2
		);
		if (HoverCard != NULL)
		{
			InvalidateRect(InvalidRect, FALSE);
		}
		else
			InvalidateRect(InvalidRect, TRUE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CUNOgameDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (HoverCard != NULL)
	{
		SetCursor(LoadCursor(NULL, IDC_HAND));
		return TRUE;
	}

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CUNOgameDlg::UpdatePlayerList()
{
	PlayersListBox.SetRedraw(FALSE);
	PlayersListBox.ResetContent();
	POSITION pos = UNOGameFlow.GetPlayers()->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < UNOGameFlow.GetPlayers()->GetCount(); i++)
	{
		player = UNOGameFlow.GetPlayers()->GetNext(pos);
		PlayersListBox.AddString(player->GetFullInfo());
	}
	PlayersListBox.SetCurSel(UNOGameFlow.GetCurrentPlayerIndex());
	PlayersListBox.SetRedraw(TRUE);
}

void CUNOgameDlg::OnBnClickedAddPlayers()
{
	CAddPlayersDlg dlgAddPlayers;
	if (dlgAddPlayers.DoModal() == IDCANCEL)
	{
		dlgAddPlayers.DeletePlayers();
	}
	else 
	{ 
		PlayerList* players = dlgAddPlayers.GetPlayers();
		UNOGameFlow.SetPlayers(players);
		UNOGameFlow.StartGame(0);
		
		UpdatePlayerList();

		RefreshDialog(TRUE);

		UNOGameFlow.StartRound(UNOGameFlow.GetCurrentPlayerIndex());

		RefreshDialog(TRUE);
	}
}

void CUNOgameDlg::OnBnClickedDrawButton()
{
	HoverCardIndex = -1;
	UNOGameFlow.DrawCard();

	if (!UNOGameFlow.CanThrowAnyCard())
	{
		DrawButton.EnableWindow(FALSE);
		NextButton.EnableWindow(TRUE);
	}
	else
	{
		DrawButton.EnableWindow(FALSE);
		NextButton.EnableWindow(FALSE);
	}
}


void CUNOgameDlg::OnBnClickedNextButton()
{
	HoverCardIndex = -1;
	DrawButton.EnableWindow(FALSE);
	NextButton.EnableWindow(FALSE);

	UNOGameFlow.GoNext();

	UpdatePlayerList();

	AdjustContolsLayout();
}


void CUNOgameDlg::OnBnClickedUnoNotCalledButton()
{
	HoverCardIndex = -1;
	UNOGameFlow.PenalizePlayersForUNOMiss();

	UpdatePlayerList();
}


void CUNOgameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (HoverCardIndex != -1)
	{
		if (!Throwing)
		{
			Throwing = true;
			int HIndex = HoverCardIndex;
			CPlayer* currPlayer = UNOGameFlow.GetCurrentPlayer();

			if (currPlayer->Deck()->GetCardsCount() >= (HIndex + 1) &&
				currPlayer == UNOGameFlow.GetPlayerWithVisibleCards())
			{
				if (UNOGameFlow.CanThrowCard(HIndex))
				{
					UNOGameFlow.ThrowCard(HIndex);
					HoverCardIndex = -1;
					HoverCard = NULL;
					LastHoverCard = NULL;

					UpdatePlayerList();

					RefreshDialog(TRUE);
				}
			}
			Throwing = false;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CUNOgameDlg::OnLbnSelchangePlayerList()
{
}

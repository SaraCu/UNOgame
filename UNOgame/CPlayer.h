#pragma once
#include "CCard.h"
#include "CDeck.h"
#include "resource.h";

class CPlayer
{
protected:
	CString Name;
	int Points;
	bool UNO = false;
	CDeck* pDeck = NULL;

public:
	CPlayer() {
		pDeck = CDeck::CreateEmpty();
		Points = 0;
		Name = _T("");
	};

	CPlayer* Clone() {
		CPlayer* player2 = new CPlayer();
		player2->Points = this->Points;
		player2->Name = this->Name;
		player2->pDeck = pDeck->Clone();
		return player2;
	}

	void Reset()
	{
		UNO = false;
		DeletePoints();
		pDeck->DeleteAllCards();
	}
	 
	virtual ~CPlayer() { delete pDeck; }
	CString GetName() { return Name; }
	void SetName(CString name) { Name = name; }

	CString GetFullInfo() {
		CString strPt;
		strPt.Format(_T("%d"), Points);
		CString strCrds;
		strCrds.Format(_T("%d"), pDeck->GetCardsCount());
		CString uno = UNO ? CString(MAKEINTRESOURCE(IDS_COMMA_UNO)) : _T("");
		return _T(" ")+ Name + CString(MAKEINTRESOURCE(IDS_COMMA_PTS)) + strPt 
			+ CString(MAKEINTRESOURCE(IDS_COMMA_CARDS)) + strCrds + _T("") + uno;
	};

	int GetPoints() { return Points; }
	void AddPoints(int points) { Points += points; }
	void DeletePoints() { Points = 0; };
	bool IsWinner() { return Points >= 500; };
	CDeck* Deck() { return pDeck; }
	void CallUNO() { UNO = true; }
	bool IsUNOCalled() { return UNO; }
	void ResetUNOCall() { UNO = false; }
};

typedef  CList<CPlayer*, CPlayer*> PlayerList;

PlayerList* ClonePlayerList(PlayerList* players);

void DeletePlayerList(PlayerList* players);


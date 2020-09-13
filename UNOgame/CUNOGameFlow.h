#pragma once
#include "CCard.h"
#include "CDeck.h"
#include "CPlayer.h"

class CUNOgameDlg;

enum class enPlayDiretion {UP, DOWN};

class CUNOGameFlow
{
protected:
	bool IsGameRunning = false;
	bool IsDealingRunning = false;
	PlayerList* Players = NULL;
	CPlayer* CurrentPlayer = NULL;
	int CurrentPlayerIndex = -1;
	CPlayer* PreviousPlayer = NULL;
	int PreviousPlayerIndex = -1;
	bool ChallangeDrawFourMode = false;
	enPlayDiretion PlayDirection = enPlayDiretion::DOWN;
	CPlayer* PlayerWithVisibleCards = NULL;
	CDeck Deck;
	CDeck DiscardPile;
	enColor DiscardColor = enColor::Any;
	CUNOgameDlg* MainDialog = NULL;

	CPlayer* GetNextPlayer();
	void Next();
	void Skip();
	void Reverse();
	void CheckRefillDeck(int cardsCountTrashold);
	int GetNextPlayerIndex();
	bool CheckChallengeCards(CPlayer* player, CCard* pileCard, enColor pileColor);
	void RoundEnd();

public:
	CUNOGameFlow() { };

	virtual ~CUNOGameFlow()
	{
		if (Players != NULL)
		{
			DeletePlayerList(Players);
			Players = NULL;
		}
	}

	void SetMainDilog(CUNOgameDlg* mainDialog)
	{
		MainDialog = mainDialog;
	}
	
	void SetPlayers(PlayerList* pPlayers) { 
		Players = pPlayers; 
		CurrentPlayer = Players->GetHead();
		CurrentPlayerIndex = 0;
		PreviousPlayer = NULL;
		PreviousPlayerIndex = -1;
	}

	PlayerList* GetPlayers() { return Players; }
	bool IsGameStarted() { return IsGameRunning; }
	bool IsRoundStarted() { return IsDealingRunning; }
	CPlayer* GetCurrentPlayer() { return CurrentPlayer; }
	int GetCurrentPlayerIndex() { return CurrentPlayerIndex; }
	CPlayer* GetPreviousPlayer() { return PreviousPlayer; }
	int GetPreviousPlayerIndex() { return PreviousPlayerIndex; }
	bool IsChallangeDrawFourMode() { return ChallangeDrawFourMode; }
	enPlayDiretion GetPlayDirection() { return PlayDirection; }
	CPlayer* GetPlayerWithVisibleCards() { return PlayerWithVisibleCards; }
	CDeck* GetDiscardPile() { return &DiscardPile; }
	enColor GetDiscardColor() { return DiscardColor; }
	void SetDiscardColor(enColor color) { DiscardColor = color; }

	void StartGame(int firstPlayerIndex);
	void InitRound(int firstPlayerIndex);
	void StartRound(int firstPlayerIndex);
	void DrawCard();
	bool CanThrowCard(int cardDeckIndex);
	void ThrowCard(int cardDeckIndex);
	void CallUNO();
	void PenalizePlayersForUNOMiss();
	CCard* GetDiscardCard();
	void GoNext();
	bool CanThrowAnyCard();
};



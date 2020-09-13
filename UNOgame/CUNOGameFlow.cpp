#include "pch.h"
#include "CUNOGameFlow.h"
#include "ColorPickerDlg.h"
#include "ChooseDlg.h"
#include "InfoDlg.h"
#include "UNOgameDlg.h"

void CUNOGameFlow::StartGame(int firstPlayerIndex)
{
	IsGameRunning = true;
	IsDealingRunning = false;

	POSITION pos = Players->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < Players->GetCount(); i++)
	{
		player = Players->GetNext(pos);
		player->Reset();
	}

	InitRound(firstPlayerIndex);
}

void CUNOGameFlow::InitRound(int firstPlayerIndex)
{
	PlayDirection = enPlayDiretion::DOWN;

	POSITION pos = Players->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < Players->GetCount(); i++)
	{
		player = Players->GetNext(pos);
		player->Deck()->DeleteAllCards();
	}

	CurrentPlayer = Players->GetAt(Players->FindIndex(firstPlayerIndex));
	CurrentPlayerIndex = firstPlayerIndex;

	PreviousPlayer = NULL;
	PreviousPlayerIndex = -1;

	PlayerWithVisibleCards = NULL;

	Deck.DeleteAllCards();
	CDeck* deck = CDeck::CreateFull();
	Deck.AddDeck(deck);
	delete deck;
	Deck.Shuffle();

	pos = Players->GetHeadPosition();
	//    player;

	for (int i = 0; i < Players->GetCount(); i++)
	{
		player = Players->GetNext(pos);
		player->ResetUNOCall();
		CardList* cards = Deck.RemoveTopCards(7);
		player->Deck()->AddCards(cards);
		player->Deck()->Sort();
		delete cards;
	}

	DiscardPile.DeleteAllCards();
	CardList* cardsDiscard;
	CCard* card;
	while (true)
	{
		cardsDiscard = Deck.RemoveTopCards(1);
		card = cardsDiscard->GetHead();
		if (card->GetType() == enCardType::DrawFour)
		{
			Deck.AddCards(cardsDiscard);
			delete cardsDiscard;
			continue;
		}
		break;
	}

	DiscardPile.AddCards(cardsDiscard);
	delete cardsDiscard;

	CCard* discardCard = GetDiscardCard();
	DiscardColor = discardCard->GetColor();

	switch (discardCard->GetType())
	{
		case enCardType::DrawTwo:
		{
			CardList* cardsTwo = Deck.RemoveTopCards(2);
			CurrentPlayer->Deck()->AddCards(cardsTwo);
			delete cardsTwo;
			if (firstPlayerIndex == Players->GetCount() - 1) firstPlayerIndex = 0;
			else firstPlayerIndex = firstPlayerIndex + 1;
			CurrentPlayer = Players->GetAt(Players->FindIndex(firstPlayerIndex));
			CurrentPlayerIndex = firstPlayerIndex;
			break;
		}
		case enCardType::Skip:
		{
			if (firstPlayerIndex == Players->GetCount() - 1) firstPlayerIndex = 0;
			else firstPlayerIndex = firstPlayerIndex + 1;
			CurrentPlayer = Players->GetAt(Players->FindIndex(firstPlayerIndex));
			CurrentPlayerIndex = firstPlayerIndex;
			break;
		}
		case enCardType::Reverse:
		{
			PlayDirection = enPlayDiretion::UP;
			if (firstPlayerIndex == 0) firstPlayerIndex = Players->GetCount() - 1;
			else firstPlayerIndex = firstPlayerIndex - 1;
			CurrentPlayer = Players->GetAt(Players->FindIndex(firstPlayerIndex));
			CurrentPlayerIndex = firstPlayerIndex;
			break;
		}
		case enCardType::Wilde:
		{
			break;
		}
	}
}

CCard* CUNOGameFlow::GetDiscardCard()
{
	return DiscardPile.GetTopCard();
}

void CUNOGameFlow::StartRound(int firstPlayerIndex)
{
	IsDealingRunning = true;

	CInfoDlg dlgInfo;
	CString info = _T("Player  ") + CurrentPlayer->GetName() + _T("  will start the round!");
	dlgInfo.SetInfo(_T("First Round..."), info);
	dlgInfo.DoModal();

	PlayerWithVisibleCards = Players->GetAt(Players->FindIndex(firstPlayerIndex));

	MainDialog->RefreshDialog(false);

	if (GetDiscardColor() == enColor::Any)
	{
		CColorPickerDlg dlgColorPickerDlg;
		dlgColorPickerDlg.SetPlayerName(GetCurrentPlayer()->GetName());
		dlgColorPickerDlg.DoModal();
		SetDiscardColor(dlgColorPickerDlg.GetColor());

		MainDialog->RefreshDialog(false);
	}

	
}

bool CUNOGameFlow::CanThrowCard(int cardDeckIndex)
{
	CCard* pcard = CurrentPlayer->Deck()->GetCardAt(cardDeckIndex);
	enCardType discCardType = DiscardPile.GetTopCard()->GetType();
	if (pcard->GetType()==enCardType::Wilde ||
		pcard->GetType() == enCardType::DrawFour) return true;
	if (pcard->GetType() == discCardType) return true;
	if (pcard->GetColor() == DiscardColor) return true;
	
	return false;
}

void CUNOGameFlow::ThrowCard(int cardDeckIndex)
{
	CCard* pileCard = GetDiscardCard();
	enColor pileColor = GetDiscardColor();

	CCard* throwCard = CurrentPlayer->Deck()->RemoveCardAt(cardDeckIndex);
	DiscardPile.Cards.AddHead(throwCard);
	DiscardColor = throwCard->GetColor();
	enCardType discardType = GetDiscardCard()->GetType();
	
	if (CurrentPlayer->Deck()->GetCardsCount() != 0)
	{
		if (discardType == enCardType::DrawFour || discardType == enCardType::Wilde)
		{
			CColorPickerDlg dlgColorPickerDlg;
			dlgColorPickerDlg.SetPlayerName(CurrentPlayer->GetName());
			dlgColorPickerDlg.DoModal();
			DiscardColor = dlgColorPickerDlg.GetColor();
		}

		if (CurrentPlayer->Deck()->GetCardsCount() == 1)
		{
			CChooseDlg clg;
			clg.SetText(_T("Call UNO!?"));
			clg.SetOkButton(_T("Yes"));
			clg.SetCancelButton(_T("NO"));
			if (clg.DoModal() == IDOK)
			{
				CurrentPlayer->CallUNO();
			}
			else
			{
				CurrentPlayer->ResetUNOCall();
			}
		}
		else
		{
			CurrentPlayer->ResetUNOCall();
		}
		
	}
	else
	{
		RoundEnd();
		
		PlayerWithVisibleCards = CurrentPlayer;

		return;
	}

	PlayerWithVisibleCards = NULL;

	MainDialog->RefreshDialog(TRUE);
	

	switch (throwCard->GetType())
	{
		case enCardType::DrawFour:
		{
			CPlayer* nextPlayer = GetNextPlayer();
			int nextPlayeIndex = GetNextPlayerIndex();

			PlayerWithVisibleCards = NULL;

			CInfoDlg dlgInfo0;
			CString info = _T("Call player  ") + nextPlayer->GetName() + _T("  for the challenge!");
			dlgInfo0.SetInfo(_T("Next player..."), info);
			dlgInfo0.DoModal();

			MainDialog->RefreshDialog(TRUE);

			CChooseDlg clg;
			info = _T("Player  ") + nextPlayer->GetName() + _T(", do you want to challenge Draw Four Wild card?");
			clg.SetText(info);
			clg.SetOkButton(_T("Yes"));
			clg.SetCancelButton(_T("NO"));
			if (clg.DoModal() == IDOK)
			{
				ChallangeDrawFourMode = true;

				PlayerWithVisibleCards = CurrentPlayer;

				MainDialog->RefreshDialog(TRUE);

				bool result = CheckChallengeCards(CurrentPlayer, pileCard, pileColor);

				CInfoDlg dlgInfoChallengeResult;
				CString info = _T("Challenge ");
				if (!result)
				{
					info += _T("FAILED!");
				}
				else
				{
					info += _T("SUCCESS!");
				}
				info += _T("!");
				dlgInfoChallengeResult.SetInfo(_T("Challenge Result..."), info);
				dlgInfoChallengeResult.DoModal();

				PlayerWithVisibleCards = NULL;
				ChallangeDrawFourMode = false;

				MainDialog->RefreshDialog(TRUE);

				if (!result)
				{
					Next();

					CheckRefillDeck(6);
					CardList* cards = Deck.RemoveTopCards(6);
					CurrentPlayer->Deck()->AddCards(cards);
					CurrentPlayer->Deck()->Sort();
					delete cards;

					Next();

					MainDialog->UpdatePlayerList();

					CInfoDlg dlgInfo1;
					info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
					dlgInfo1.SetInfo(_T("Next player..."), info);
					dlgInfo1.DoModal();
				}
				else
				{
					CheckRefillDeck(4);
					CardList* cards = Deck.RemoveTopCards(4);
					CurrentPlayer->Deck()->AddCards(cards);
					CurrentPlayer->Deck()->Sort();
					delete cards;

					Next();
				}

				ChallangeDrawFourMode = false;
			}
			else
			{
				Next();

				CheckRefillDeck(4);
				CardList* cards = Deck.RemoveTopCards(4);
				nextPlayer->Deck()->AddCards(cards);
				nextPlayer->Deck()->Sort();
				delete cards;

				Next();

				PlayerWithVisibleCards = NULL;
				MainDialog->RefreshDialog(TRUE);
				MainDialog->UpdatePlayerList();

				CInfoDlg dlgInfo2;
				info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
				dlgInfo2.SetInfo(_T("Next player..."), info);
				dlgInfo2.DoModal();
			}
			break;
		}

		case enCardType::Skip:
		{
			PlayerWithVisibleCards = NULL;
			MainDialog->RefreshDialog(TRUE);

			Skip();

			if (Players->GetCount() != 2)
			{
				MainDialog->UpdatePlayerList();

				CInfoDlg dlgInfo3;
				CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
				dlgInfo3.SetInfo(_T("Next player..."), info);
				dlgInfo3.DoModal();
			}
			break;
		}
		
		case enCardType::Reverse:
		{
			PlayerWithVisibleCards = NULL;
			MainDialog->RefreshDialog(TRUE);

			if (Players->GetCount() == 2)
			{
				if (PlayDirection == enPlayDiretion::DOWN)
				{
					PlayDirection = enPlayDiretion::UP;
				}
				else
				{
					PlayDirection = enPlayDiretion::DOWN;
				}
				Skip();
			}
			else
			{
				Reverse();

				MainDialog->UpdatePlayerList();

				CInfoDlg dlgInfo4;
				CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
				dlgInfo4.SetInfo(_T("Next player..."), info);
				dlgInfo4.DoModal();
			}
			break;
		}
		
		case enCardType::DrawTwo:
		{
			PlayerWithVisibleCards = NULL;
			MainDialog->RefreshDialog(TRUE);

			Next();

			CheckRefillDeck(2);
			CardList* cardsTwo = Deck.RemoveTopCards(2);
			CurrentPlayer->Deck()->AddCards(cardsTwo);
			CurrentPlayer->Deck()->Sort();
			delete cardsTwo;

			Next();

			if (Players->GetCount() != 2)
			{
				MainDialog->UpdatePlayerList();

				CInfoDlg dlgInfo5;
				CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
				dlgInfo5.SetInfo(_T("Next player..."), info);
				dlgInfo5.DoModal();
			}
			break;
		}

		case enCardType::Wilde:
		{
			PlayerWithVisibleCards = NULL;
			MainDialog->RefreshDialog(TRUE);
			
			Next();

			MainDialog->UpdatePlayerList();

			CInfoDlg dlgInfo6;
			CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
			dlgInfo6.SetInfo(_T("Next player..."), info);
			dlgInfo6.DoModal();
			break;
		}

		default:
		{
			PlayerWithVisibleCards = NULL;
			MainDialog->RefreshDialog(TRUE);

			Next();
			
			MainDialog->UpdatePlayerList();

			CInfoDlg dlgInfo7;
			CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
			dlgInfo7.SetInfo(_T("Next player..."), info);
			dlgInfo7.DoModal();		
		}
	}
	
	PlayerWithVisibleCards = CurrentPlayer;
}

void CUNOGameFlow::RoundEnd()
{
	CPlayer* roundWinner = CurrentPlayer;
	int idxNext = CurrentPlayerIndex;

	idxNext++;
	if (idxNext == Players->GetCount()) idxNext = 0;

	POSITION pos = Players->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < Players->GetCount(); i++)
	{
		player = Players->GetNext(pos);
		if (player != roundWinner)
		{
			roundWinner->AddPoints(player->Deck()->CalculateDeckPoints());
		}
	}

	int FechPlusX = 0;
	if (DiscardPile.GetTopCard()->GetType() == enCardType::DrawFour)
	{
		FechPlusX = 4;
	}
	if (DiscardPile.GetTopCard()->GetType() == enCardType::DrawTwo)
	{
		FechPlusX = 2;
	}
	if (FechPlusX > 0)
	{
		CheckRefillDeck(FechPlusX);
		CardList* cards = Deck.RemoveTopCards(FechPlusX);
		pos = cards->GetHeadPosition();
		CCard* card;
		for (int i = 0; i < cards->GetCount(); i++)
		{
			card = cards->GetNext(pos);
			roundWinner->AddPoints(card->GetPoints());
		}
		delete cards;
	}

	MainDialog->UpdatePlayerList();
	MainDialog->RefreshDialog(TRUE);
	
	CInfoDlg dlgInfo;

	if (roundWinner->GetPoints() >= 500)
	{
		CString info = _T("Game Winner is  ") + roundWinner->GetName() + _T(" ! Congratulations!");
		dlgInfo.SetInfo(_T("Winner is..."), info);
		dlgInfo.SetWinnerFlag(true);
		dlgInfo.DoModal();
		dlgInfo.SetWinnerFlag(false);

		StartGame(idxNext);
		StartRound(idxNext);
	}
	else
	{
		CString info = _T("Round Winner is  ") + roundWinner->GetName() + _T(" !");
		dlgInfo.SetInfo(_T("Winner is..."), info);
		dlgInfo.SetRoundWinnerFlag(true);
		dlgInfo.DoModal();
		dlgInfo.SetRoundWinnerFlag(false);

		InitRound(idxNext);
		StartRound(idxNext);
	}

	MainDialog->RefreshDialog(TRUE);
}

bool CUNOGameFlow::CheckChallengeCards(CPlayer* player, CCard* pileCard, enColor pileColor)
{
	POSITION pos = player->Deck()->Cards.GetHeadPosition();
	CCard* card;
	for (int i = 0; i < player->Deck()->Cards.GetCount(); i++)
	{
		card = player->Deck()->Cards.GetNext(pos);
		if (card->GetType() != enCardType::DrawFour)
		{
			if (pileCard->GetType() == card->GetType()) return true;
			if (pileCard->GetColor() == card->GetColor()) return true;
		}
	}
	return false;
}

CPlayer* CUNOGameFlow::GetNextPlayer()
{
	int index = GetNextPlayerIndex();
	CPlayer* player = Players->GetAt(Players->FindIndex(index));
	return player;
}

int CUNOGameFlow::GetNextPlayerIndex()
{
	int index = CurrentPlayerIndex;
	if (PlayDirection == enPlayDiretion::DOWN)
	{
		if (CurrentPlayerIndex + 1 == Players->GetCount())
		{
			index = 0;
		}
		else
		{
			index++;
		}
	}
	else
	{
		if (CurrentPlayerIndex == 0)
		{
			index = Players->GetCount() - 1;
		}
		else
		{
			index--;
		}
	}
	return index;
}

void CUNOGameFlow::Next()
{
	PreviousPlayerIndex = CurrentPlayerIndex;
	PreviousPlayer = CurrentPlayer;

	CurrentPlayerIndex = GetNextPlayerIndex();
	CurrentPlayer = Players->GetAt(Players->FindIndex(CurrentPlayerIndex));
	
	PlayerWithVisibleCards = NULL;
}

void CUNOGameFlow::Skip()
{
	Next();
	Next();
}

void CUNOGameFlow::Reverse()
{
	if (PlayDirection == enPlayDiretion::DOWN)
	{
		PlayDirection = enPlayDiretion::UP;
	}
	else
	{
		PlayDirection = enPlayDiretion::DOWN;
	}
	Next();
}

void CUNOGameFlow::DrawCard()
{
	CheckRefillDeck(1);
	CardList* cards = Deck.RemoveTopCards(1);
	CurrentPlayer->Deck()->AddCards(cards);
	delete cards;

	MainDialog->RefreshDialog(TRUE);
}

void CUNOGameFlow::GoNext()
{
	PlayerWithVisibleCards = NULL;
	MainDialog->RefreshDialog(TRUE);

	if (CurrentPlayer->Deck()->GetCardsCount() != 1) CurrentPlayer->ResetUNOCall();

	Next();

	CInfoDlg dlgInfo;
	CString info = _T("Please call player  ") + CurrentPlayer->GetName() + _T(" !");
	dlgInfo.SetInfo(_T("Next player..."), info);
	dlgInfo.DoModal();

	PlayerWithVisibleCards = CurrentPlayer;

	CurrentPlayer->Deck()->Sort();
	PreviousPlayer->Deck()->Sort();

	MainDialog->RefreshDialog(TRUE);
}

void CUNOGameFlow::CallUNO()
{
	CurrentPlayer->CallUNO();
}

void CUNOGameFlow::PenalizePlayersForUNOMiss()
{
	POSITION pos = Players->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < Players->GetCount(); i++)
	{
		player = Players->GetNext(pos);
		if (player->Deck()->GetCardsCount() == 1 && !player->IsUNOCalled())
		{
			CheckRefillDeck(2);
			CardList* cards = Deck.RemoveTopCards(2);
			player->Deck()->AddCards(cards);
			player->Deck()->Sort();
			delete cards;
		}
	}
}

void CUNOGameFlow::CheckRefillDeck(int cardsCountTrashold)
{
	if (Deck.GetCardsCount() <= cardsCountTrashold)
	{
		CardList* cards = DiscardPile.RemoveTopCards(1);
		Deck.AddDeck(&DiscardPile);
		DiscardPile.AddCards(cards);
		delete cards;
		Deck.Shuffle();
	}
}

bool CUNOGameFlow::CanThrowAnyCard()
{
	CPlayer* currPlayer = CurrentPlayer;
	enCardType disType = DiscardPile.GetTopCard()->GetType();
	enColor disColor = DiscardColor;

	POSITION pos = currPlayer->Deck()->Cards.GetHeadPosition();
	CCard* card;
	for (int i = 0; i < currPlayer->Deck()->Cards.GetCount(); i++)
	{
		card = currPlayer->Deck()->Cards.GetNext(pos);

		if (card->GetType() == enCardType::Wilde) return true;
		if (card->GetType() == enCardType::DrawFour) return true;
		if (disType == card->GetType()) return true;
		if (disColor == card->GetColor()) return true;
	}
	return false;
}


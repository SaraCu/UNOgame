#pragma once

#include "CCard.h"

class CDeck
{
public:
	CDeck() {}
	virtual ~CDeck()
	{
		DeleteAllCards();
	}

	static CDeck* CreateFull();
	static CDeck* CreateEmpty() { return new CDeck(); };

	CardList Cards;
	int GetCardsCount() {
		return Cards.GetCount();
	}

	int CalculateDeckPoints();
	CCard* GetTopCard();
	CCard* GetCardAt(int index);
	void AddCards(CCard* pCard);
	void AddCards(CardList* pCards);
	void AddDeck(CDeck* pDeck);
	CCard* RemoveCardAt(int index);
	CardList* RemoveTopCards(int count);
	CardList* RemoveTopTwoCards();
	CardList* RemoveTopFourCards();
	void DeleteAllCards();
	void Shuffle();
	void Sort();

	CDeck* Clone()
	{
		CDeck* deck2 = CDeck::CreateEmpty();
		POSITION pos = Cards.GetHeadPosition();
		CCard* card2;
		for (int i = 0; i < Cards.GetCount(); i++)
		{
			card2 = Cards.GetNext(pos);
			deck2->Cards.AddTail(card2->Clone());
		}
		return deck2;
	}
};


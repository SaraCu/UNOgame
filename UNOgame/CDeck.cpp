#include "pch.h"
#include "CDeck.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>       

static bool sRndSet = false;

CDeck* CDeck::CreateFull()
{
	CDeck* deck = new CDeck();
	int i,j,z;

	for (i = 0; i < 4; i++)
	{
		CCard* card = new CCard(enCardType::N0, (enColor)i);
		deck->Cards.AddTail(card);
	}

	for (z = 0; z < 2; z++)
	{
		for (j = (int)enCardType::N1; j <= (int)enCardType::DrawTwo; j++)
		{
			for (i = 0; i < 4; i++)
			{
				CCard* card = new CCard((enCardType)j, (enColor)i);
				deck->Cards.AddTail(card);
			}
		}
	}

	for (z = 0; z < 4; z++)
	{
		CCard* card = new CCard(enCardType::Wilde, enColor::Any);
		deck->Cards.AddTail(card);
	}

	for (z = 0; z < 4; z++)
	{
		CCard* card = new CCard(enCardType::DrawFour, enColor::Any);
		deck->Cards.AddTail(card);
	}

	return deck;
}

CCard* CDeck::GetTopCard() 
{
	return Cards.GetHead();
}

CCard* CDeck::GetCardAt(int index)
{
	return Cards.GetAt(Cards.FindIndex(index));
}

void CDeck::AddCards(CCard* pCard)
{
	Cards.AddTail(pCard);
}

void CDeck::AddCards(CardList* pCards)
{
	Cards.AddTail(pCards);
}

void CDeck::AddDeck(CDeck* pDeck)
{
	Cards.AddTail(&(pDeck->Cards));
	pDeck->Cards.RemoveAll();
}

CCard* CDeck::RemoveCardAt(int index)
{
	CCard* card = GetCardAt(index);
	Cards.RemoveAt(Cards.FindIndex(index));
	return card;
}

CardList* CDeck::RemoveTopCards(int count)
{
	if (Cards.GetCount() >= count)
	{
		CardList* pCards = new CardList();
		for (int i = 0; i < count; i++)
		{
			CCard* card = GetTopCard();
			Cards.RemoveHead();
			pCards->AddTail(card);
		}
		return pCards;
	}

	return NULL;
}

CardList* CDeck::RemoveTopTwoCards()
{
	return RemoveTopCards(2);
}

CardList* CDeck::RemoveTopFourCards()
{
	return RemoveTopCards(4);
}

void CDeck::DeleteAllCards()
{
	if (Cards.GetCount() == 0) return;
	
	CCard* card;
	POSITION pos = Cards.GetHeadPosition();
	for (int i = 0; i < Cards.GetCount(); i++)
	{
		card = Cards.GetNext(pos);
		delete card;
	}

	Cards.RemoveAll();
}

int CDeck::CalculateDeckPoints()
{
	int points = 0;
	
	CCard* card;
	POSITION pos = Cards.GetHeadPosition();
	for (int i = 0; i < Cards.GetCount(); i++)
	{
		card = Cards.GetNext(pos);
		points += card->GetPoints();
	}

	return points;
}

void CDeck::Shuffle()
{
	if (!sRndSet) 
	{
		srand((unsigned int)time(NULL)); 
		sRndSet = true;
	}

	int cardsCount = GetCardsCount();
	CDeck* shuffledDeck = CDeck::CreateEmpty();
	while (shuffledDeck->GetCardsCount() != cardsCount)
	{
		long l = ((long) GetCardsCount()) * rand() / RAND_MAX - 1;
		if (l < 0 || GetCardsCount() == 1)
		{
			l = 0; 
		}

		shuffledDeck->AddCards(Cards.GetAt(Cards.FindIndex(l)));
		Cards.RemoveAt(Cards.FindIndex(l));
	}

	for (int i = 0; i < cardsCount; i++)
	{
		AddCards(shuffledDeck->Cards.GetHead());
		shuffledDeck->Cards.RemoveHead();
	}

	delete shuffledDeck;
}

void CDeck::Sort()
{
	CDeck* sorteddDeck = CDeck::CreateEmpty();

	for (int iSortWeight = 1; iSortWeight <= CCard::MaxSortWeight; iSortWeight++)
	{
		CCard* card;
		POSITION pos = Cards.GetHeadPosition();
		for (int i = 0; i < Cards.GetCount(); i++)
		{
			card = Cards.GetNext(pos);
			if (iSortWeight == card->GetSortWeight())
			{
				sorteddDeck->AddCards(card);
			}
		}
	}

	Cards.RemoveAll();
	AddDeck(sorteddDeck);
	sorteddDeck->Cards.RemoveAll();
	delete sorteddDeck;
}
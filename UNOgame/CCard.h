#pragma once

enum class enCardType { N0 = 0, N1, N2, N3, N4, N5, N6, N7, N8, N9, Skip, Reverse, DrawTwo, DrawFour, Wilde};

enum class enColor { Red = 0, Yellow, Green, Blue, Any };

class CCard
{
	friend class CDeck;

private:
	CCard() {
		Type = enCardType::N0;
		Color = enColor::Red;
		SortWeight = 0;
		Points = 0;
	};

	CCard* Clone()
	{
		CCard* card2 = new CCard();
		card2->Type = Type;
		card2->Color = Color;
		card2->SortWeight = SortWeight;
		card2->Points = Points;
		return card2;
	}

	void CalculateSortWeight()
	{
		if (Color != enColor::Any)
		{
			SortWeight = ((int)Type + 1) + ((int)Color) * 13;
		}
		else
		{
			if (Type == enCardType::Wilde)
			{
				SortWeight = 13 * 4 + 1;
			}
			else
			{
				SortWeight = 13 * 4 + 2;
			}
		}
	}

	void CalculatePoints()
	{
		switch (Type)
		{
		case enCardType::Skip:
		case enCardType::Reverse:
		case enCardType::DrawTwo:
			Points = 20;
			return;
		case enCardType::DrawFour:
		case enCardType::Wilde:
			Points = 50;
			return;
		default:
			Points = (int) Type;
			return;
		}
	}

	enCardType Type;
	enColor Color;
	int Points;
	int SortWeight;
	static const int MaxSortWeight = 13 * 4 + 2;
	int GetSortWeight() { return SortWeight; }

public:
	CCard(enCardType type, enColor color)
	{
		Type = type;
		if (enCardType::DrawFour == type || enCardType::Wilde == type)
		{
			Color = enColor::Any;
		}
		else
		{
			Color = color;
		}

		CalculateSortWeight();
		CalculatePoints();
	}

	enCardType GetType() { return Type; }
	enColor GetColor() { return Color; }
	int GetPoints() { return Points; }
};

#define CardList CList<CCard*, CCard*>
#include "pch.h"
#include "CPlayer.h"

PlayerList* ClonePlayerList(PlayerList* playersSource)
{
	PlayerList* players2 = new PlayerList();
	POSITION pos = playersSource->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < playersSource->GetCount(); i++)
	{
		player = playersSource->GetNext(pos);
		players2->AddTail(player->Clone());
	}

	return players2;
};

void DeletePlayerList(PlayerList* players)
{
	POSITION pos = players->GetHeadPosition();
	CPlayer* player;
	for (int i = 0; i < players->GetCount(); i++)
	{
		player = players->GetNext(pos);
		delete player;
	}

	delete players;
}
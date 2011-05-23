#include "PKTK.h"
#include "../../D2Ptrs.h"
#include "../../BH.h"

#include "../../D2Ptrs.h"
#include "Constants.h"

map<std::string, Toggle> PKTK::Toggles;

using namespace Drawing;

/*
[03:27:33pm]	<lord2800>	ok, what about other common features? autoaim is obvious, I also plan to add auto corpse pickup
[03:27:48pm]	<Gary13579>	potting from stash
[03:28:17pm]	<Gary13579>	blind spot teleporting
[03:28:30pm]	<lord2800>	what's that?
[03:28:46pm]	<Gary13579>	if you teleport on a hammerdin, it adjusts the coords of the tele to teleport into their blindspot
[03:29:07pm]	triwhirl would be nice
[03:29:11pm]	<lord2800>	triwhirl?
[03:29:24pm]	<Gary13579>	perfect triangle whirlwind to maximize damage
[03:32:54pm]	<Gary13579>	what about auto dodging
[03:30:18pm]	<Gary13579>	if you release it
[03:30:23pm]	could you include "TPPK" in feature list?
*/

static BYTE Bps[7][4] = {
	{  20, 100, 100, 150 },
	{ 100, 100, 100, 200 },
	{ 200, 200, 200, 200 },
	{ 100, 100, 100, 100 },
	{  50, 100, 100,  50 },
	{ 100, 200, 200, 100 },
	{ 100, 200, 200, 200 }
};

void PKTK::OnLoad()
{
}

void PKTK::OnUnload()
{
}

void PKTK::OnLoop()
{
}

void PKTK::OnKey(bool up, BYTE key, LPARAM lParam, bool* block)
{
}

void PKTK::OnLeftClick(bool up, int x, int y, bool* block)
{
}

void PKTK::OnGameJoin(const string& name, const string& pass, int diff)
{
	UnitAny* player = D2CLIENT_GetPlayerUnit();
	DWORD cType = player->dwTxtFileNo;
	static BYTE ids[] = { 105, 102, 99, 93 };
	for(int i = 0; i < 4; i++)
	{
		BYTE packet[] = { 0x1D, ids[i], Bps[cType][i] };
		D2NET_ReceivePacket(packet, sizeof(packet));
	}
}

void PKTK::OnGameExit()
{
}

void PKTK::OnDraw()
{
}

void PKTK::OnGamePacketRecv(BYTE* packet, bool* block)
{
	std::map<BYTE, BYTE> indexes;
	indexes[105] = 0;
	indexes[102] = 1;
	indexes[99]  = 2;
	indexes[93]  = 3;

	switch(packet[0])
	{
		case 0x1D: case 0x1E: case 0x1F: {
			DWORD cType = D2CLIENT_GetPlayerUnit()->dwTxtFileNo;
			if(indexes.find(packet[1]) != indexes.end())
				if(packet[2] < Bps[cType][indexes[packet[1]]])
					*block = true;
			break;
		}
		case 0x15: {
			UnitAny* Player = D2CLIENT_GetPlayerUnit();
			if(*(DWORD*)&packet[2] == Player->dwUnitId) {
				packet[10] = 0;

				if(Player->dwMode == PLAYER_MODE_DEATH || 
						Player->dwMode == PLAYER_MODE_STAND_OUTTOWN ||
						Player->dwMode == PLAYER_MODE_WALK_OUTTOWN || 
						Player->dwMode == PLAYER_MODE_RUN || 
						Player->dwMode == PLAYER_MODE_STAND_INTOWN || 
						Player->dwMode == PLAYER_MODE_WALK_INTOWN ||
						Player->dwMode == PLAYER_MODE_DEAD || 
						Player->dwMode == PLAYER_MODE_SEQUENCE ||
						Player->dwMode == PLAYER_MODE_BEING_KNOCKBACK)
					break;

				if(Player->dwFrameRemain > 10)
					Player->dwFrameRemain = 10;
			}
			break;
		}
		case 0x9C: {
			BYTE ear = packet[9] & 0x80;
			if(ear) *block = true;
			else {
				//DWORD code = *(DWORD*)packet[15];
				//if(code == 0x65617220)
				//	*block = true;
			}
			break;
		}
		case 0xA7:
			if(packet[6] == AFFECT_JUST_PORTALED)
				*block = true;
			break;
		case 0xAA:
			*(DWORD*)(packet+packet[6]) = -1;
			packet[6] += 4;
			break;
	}
}

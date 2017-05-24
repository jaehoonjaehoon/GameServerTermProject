#include "stdafx.h"
#include "NPC.h"

#include "ThreadFunctions.h"
#include "Player.h"

NPC::NPC()
{
	near_list.reserve(100);
	near_sector.reserve(5);
	curr_sector = nullptr;
}

NPC::~NPC()
{
}

void NPC::HeartBeat()
{
retry:
	int dir = rand() % 4;

	switch (dir) {
	case 0:
		if (x > 0)
			--x;
		else 
			goto retry;
		break;
		
	case 1:
		if (x < BOARD_WIDTH - 1) 
			++x;
		else
			goto retry;
		break;

	case 2:
		if (y > 0)
			--y;
		else
			goto retry;
		break;

	case 3:
		if (y < BOARD_HEIGHT - 1) 
			++y;
		else 
			goto retry;
		break;
	}

	// 이동 후 섹터 업데이트
	zone.UpdateSector(id);

	// view에 기존의 주변 플레이어만 삽입
	view.clear();

	for (auto near_id : near_list)
	{
		if (near_id < MAX_USER)
			view.insert(near_id);
	}

	// 새로운 주변 플레이어 리스트 업데이트
	UpdateNearList();

	// temp에 새롭게 구한 주변 플레이어 삽입
	set<UINT> temp;

	for (auto near_id : near_list)
	{
		if (near_id < MAX_USER)
			temp.insert(near_id);
	}

	// view와 temp를 비교하여 추가/이동/삭제된 플레이어 판별
	for (auto player_id : view)
	{
		Player* player = dynamic_cast<Player*>(objects[player_id]);

		int count = temp.erase(player_id);

		if (player->GetUsing() == false)
			continue;

		if (count != 0)
		{
			// view와 temp에 모두 있을 때 (이동)
			sc_packet_pos pos_packet;
			pos_packet.size = sizeof(pos_packet);
			pos_packet.type = SC_POS;
			pos_packet.id = id;
			pos_packet.x = x;
			pos_packet.y = y;

			SendPacket(player, reinterpret_cast<BYTE*>(&pos_packet));
		}

		else
		{
			// view에는 있는데 temp에는 없을 때 (삭제)
			sc_packet_remove_player remove_packet;
			remove_packet.size = sizeof(remove_packet);
			remove_packet.type = SC_REMOVE_PLAYER;
			remove_packet.id = id;

			SendPacket(player, reinterpret_cast<BYTE*>(&remove_packet));
		}
	}

	// 위에서 temp.erase(id)를 하며 검사했기 때문에 temp에는 추가된 객체만 존재
	for (auto player_id : temp)
	{
		Player* player = dynamic_cast<Player*>(objects[player_id]);

		sc_packet_put_player put_packet;
		put_packet.size = sizeof(put_packet);
		put_packet.type = SC_PUT_PLAYER;
		put_packet.id = id;
		put_packet.x = x;
		put_packet.y = y;

		SendPacket(player, reinterpret_cast<BYTE*>(&put_packet));
	}
}
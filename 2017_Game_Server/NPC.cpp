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

	// �̵� �� ���� ������Ʈ
	zone.UpdateSector(id);

	// view�� ������ �ֺ� �÷��̾ ����
	view.clear();

	for (auto near_id : near_list)
	{
		if (near_id < MAX_USER)
			view.insert(near_id);
	}

	// ���ο� �ֺ� �÷��̾� ����Ʈ ������Ʈ
	UpdateNearList();

	// temp�� ���Ӱ� ���� �ֺ� �÷��̾� ����
	set<UINT> temp;

	for (auto near_id : near_list)
	{
		if (near_id < MAX_USER)
			temp.insert(near_id);
	}

	// view�� temp�� ���Ͽ� �߰�/�̵�/������ �÷��̾� �Ǻ�
	for (auto player_id : view)
	{
		Player* player = dynamic_cast<Player*>(objects[player_id]);

		int count = temp.erase(player_id);

		if (player->GetUsing() == false)
			continue;

		if (count != 0)
		{
			// view�� temp�� ��� ���� �� (�̵�)
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
			// view���� �ִµ� temp���� ���� �� (����)
			sc_packet_remove_player remove_packet;
			remove_packet.size = sizeof(remove_packet);
			remove_packet.type = SC_REMOVE_PLAYER;
			remove_packet.id = id;

			SendPacket(player, reinterpret_cast<BYTE*>(&remove_packet));
		}
	}

	// ������ temp.erase(id)�� �ϸ� �˻��߱� ������ temp���� �߰��� ��ü�� ����
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
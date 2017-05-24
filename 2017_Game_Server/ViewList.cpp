#include "stdafx.h"
#include "ViewList.h"

#include "ThreadFunctions.h"
#include "Player.h"

ViewList::ViewList(Player* player)
	: owner(player)
{
	view.reserve(view_size);
	temp.reserve(view_size);
}

ViewList::~ViewList()
{
}

void ViewList::Update(vector<UINT>& near_list)
{
	temp.clear();

	//----------------------------------------------------------------------------
	// 여기서 Read Lock을 쓰면 될 것 같다.
	{
		Lock sync(this);

		temp.resize(view.size());

		// 현재 view에 존재하는 오브젝트의 마킹을 모두 false로 복사
		transform(view.begin(), view.end(), temp.begin(), [](pair<UINT, bool>& data) {
			data.second = false;
			return data;
		});
	}
	//----------------------------------------------------------------------------

	// near_list에 존재하는 오브젝트 아이디를 temp에 추가
	// near_list는 Update된 상태
	for (auto near_id : near_list)
	{
		// 이미 temp에 존재하는 아이디라면 true로 마킹
		auto iter = find_if(temp.begin(), temp.end(), [=](pair<UINT, bool>& data) {
			if (data.first == near_id)
			{
				data.second = true;
				return true;
			}
			return false;
		});

		// near_list에만 있는 아이디를 temp에 추가
		if (iter == temp.end())
			InsertInTemp(near_id);

		// 플레이어라면
		if (near_id < MAX_USER)
		{
			// 상대 view_list에 내가 있는지 확인
			Player* other = dynamic_cast<Player*>(objects[near_id]);

			if (other->GetViewList().IsInViewList(owner->GetID()) == true)
			{
				// 상대방의 view_list에도 내가 있음
				// 상대방에게 내가 이동한 것을 알림
				other->GetViewList().MoveMessage(owner->GetID());
			}

			else
			{
				// 상대방의 view_list에는 내가 없음
				// 상대방에게 내가 시야 내로 들어온 것을 알림
				other->GetViewList().InsertInView(owner->GetID());
			}
		}
	}

	// 이제 tmp_list에만 있고 near_list에 없는 애들은 마킹이 false
	// 즉, 시야 내에 있다가 시야 밖으로 사라진 오브젝트들
	auto remove_iter = remove_if(temp.begin(), temp.end(), [&](pair<UINT, bool>& data) {
		if (data.second == false)
		{
			RemoveMessage(data.first);

			// 플레이어라면
			if (data.first < MAX_USER)
			{
				// 상대방의 view_list에서도 나를 지워줘야함
				Player* other = dynamic_cast<Player*>(objects[data.first]);
				other->GetViewList().RemoveInView(owner->GetID());
			}

			return true;
		}
		return false;
	});

	// 이제 나 자신에게 remove packet도 전부 보냈고,
	// 상대방에게도 나를 지우라고 알렸으니 나의 view_list에서만
	// 삭제를 하면 완성
	temp.erase(remove_iter, temp.end());

	// 작업을 마친 temp_view와 curr_view를 swap
	Lock sync(this);
	view.swap(temp);
}

bool ViewList::IsInViewList(const UINT id)
{
	Lock sync(this);

	auto iter = find_if(view.begin(), view.end(), [=](pair<UINT, bool>& data) {
		return data.first == id;
	});

	return (iter != view.end());
}

void ViewList::InsertInTemp(const UINT id)
{
	if (temp.size() >= view_size)
		return;

	temp.push_back(make_pair(id, true));

	PutMessage(id);
}

void ViewList::InsertInView(const UINT id)
{
	Lock sync(this);

	if (view.size() >= view_size)
		return;

	view.push_back(make_pair(id, true));

	PutMessage(id);
}

void ViewList::RemoveInView(const UINT id)
{
	Lock sync(this);

	auto iter = find_if(view.begin(), view.end(), [=](pair<UINT, bool>& data) {
		if (data.first == id)
		{
			data.second = true;
			return true;
		}
		return false;
	});

	if (iter != view.end())
		view.erase(iter);

	RemoveMessage(id);
}

void ViewList::PutMessage(const UINT id)
{
	sc_packet_put_player put_packet;
	put_packet.size = sizeof(put_packet);
	put_packet.type = SC_PUT_PLAYER;
	put_packet.id = id;
	put_packet.x = objects[id]->GetX();
	put_packet.y = objects[id]->GetY();

	SendPacket(owner, reinterpret_cast<BYTE*>(&put_packet));
}

void ViewList::MoveMessage(const UINT id)
{
	sc_packet_pos pos_packet;
	pos_packet.size = sizeof(pos_packet);
	pos_packet.type = SC_POS;
	pos_packet.id = id;
	pos_packet.x = objects[id]->GetX();
	pos_packet.y = objects[id]->GetY();

	SendPacket(owner, reinterpret_cast<BYTE*>(&pos_packet));
}

void ViewList::RemoveMessage(const UINT id)
{
	sc_packet_remove_player remove_packet;
	remove_packet.size = sizeof(remove_packet);
	remove_packet.type = SC_REMOVE_PLAYER;
	remove_packet.id = id;

	SendPacket(owner, reinterpret_cast<BYTE*>(&remove_packet));
}

void ViewList::Clear()
{
	// 주변의 모든 플레이어들에게 내가 나가는 것을 알림
	for (auto data : view)
	{
		if (data.first < MAX_USER)
		{
			Player* other = dynamic_cast<Player*>(objects[data.first]);
			other->GetViewList().RemoveInView(owner->GetID());
		}
	}

	view.clear();
	temp.clear();
}
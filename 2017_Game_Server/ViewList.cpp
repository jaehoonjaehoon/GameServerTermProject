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
	// ���⼭ Read Lock�� ���� �� �� ����.
	{
		Lock sync(this);

		temp.resize(view.size());

		// ���� view�� �����ϴ� ������Ʈ�� ��ŷ�� ��� false�� ����
		transform(view.begin(), view.end(), temp.begin(), [](pair<UINT, bool>& data) {
			data.second = false;
			return data;
		});
	}
	//----------------------------------------------------------------------------

	// near_list�� �����ϴ� ������Ʈ ���̵� temp�� �߰�
	// near_list�� Update�� ����
	for (auto near_id : near_list)
	{
		// �̹� temp�� �����ϴ� ���̵��� true�� ��ŷ
		auto iter = find_if(temp.begin(), temp.end(), [=](pair<UINT, bool>& data) {
			if (data.first == near_id)
			{
				data.second = true;
				return true;
			}
			return false;
		});

		// near_list���� �ִ� ���̵� temp�� �߰�
		if (iter == temp.end())
			InsertInTemp(near_id);

		// �÷��̾���
		if (near_id < MAX_USER)
		{
			// ��� view_list�� ���� �ִ��� Ȯ��
			Player* other = dynamic_cast<Player*>(objects[near_id]);

			if (other->GetViewList().IsInViewList(owner->GetID()) == true)
			{
				// ������ view_list���� ���� ����
				// ���濡�� ���� �̵��� ���� �˸�
				other->GetViewList().MoveMessage(owner->GetID());
			}

			else
			{
				// ������ view_list���� ���� ����
				// ���濡�� ���� �þ� ���� ���� ���� �˸�
				other->GetViewList().InsertInView(owner->GetID());
			}
		}
	}

	// ���� tmp_list���� �ְ� near_list�� ���� �ֵ��� ��ŷ�� false
	// ��, �þ� ���� �ִٰ� �þ� ������ ����� ������Ʈ��
	auto remove_iter = remove_if(temp.begin(), temp.end(), [&](pair<UINT, bool>& data) {
		if (data.second == false)
		{
			RemoveMessage(data.first);

			// �÷��̾���
			if (data.first < MAX_USER)
			{
				// ������ view_list������ ���� ���������
				Player* other = dynamic_cast<Player*>(objects[data.first]);
				other->GetViewList().RemoveInView(owner->GetID());
			}

			return true;
		}
		return false;
	});

	// ���� �� �ڽſ��� remove packet�� ���� ���°�,
	// ���濡�Ե� ���� ������ �˷����� ���� view_list������
	// ������ �ϸ� �ϼ�
	temp.erase(remove_iter, temp.end());

	// �۾��� ��ģ temp_view�� curr_view�� swap
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
	// �ֺ��� ��� �÷��̾�鿡�� ���� ������ ���� �˸�
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
#pragma once

class Player;

void AcceptThread();
void WorkerThread();

void AfterRecv(const UINT id, const int transferred);

void SendPacket(const UINT id, BYTE* packet);
void SendPacket(Player* player, BYTE* packet);

void ProcessPacket(const UINT id, BYTE* packet);
void ProcessEvent(const NPC_EVENT& e);

void UpdatePlayerView(const UINT id);

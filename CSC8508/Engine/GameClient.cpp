#include "GameClient.h"
#include "NetworkManager.h"

#include <iostream>
#include <string>

using namespace NCL;
using namespace CSC8508;

GameClient::GameClient(NetworkManager* manager)	{
	netHandle = enet_host_create(nullptr, 1, 1, 0, 0);
	this->manager = manager;
}

GameClient::~GameClient()	{
}

bool GameClient::Connect(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int portNum) {
	ENetAddress address;
	address.port = portNum;

	address.host = (d << 24) | (c << 16) | (b << 8) | (a);

	netPeer = enet_host_connect(netHandle, &address, 2, 0);

	if (netPeer != nullptr) {
		//threadAlive = true;
		//updateThread = std::thread(&GameClient::ThreadedUpdate, this);
	}

	return netPeer != nullptr;
}

void GameClient::UpdateClient() {
	if (netHandle == nullptr)
	{
		return;
	}
	//Handle all incoming packets & send any packets awaiting dispatch
	ENetEvent event;
	while (enet_host_service(netHandle, &event, 0) > 0)
	{
		if (event.type == ENET_EVENT_TYPE_CONNECT) {
			std::cout << "Client: Connected to server!" << std::endl;
			
		
			
		}
		else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
			//std::cout << "Client: Packet recieved..." << std::endl;
			GamePacket* packet = (GamePacket*)event.packet->data;
			ProcessPacket(packet);

			if (packet->type == Player_Connected) 
				if(id==-1) id = ((NewPlayerPacket*)packet)->playerID;
			if (packet->type == Player_Count) 
				UpdateClientLobby(((PlayerCountPacket*)packet)->playerIDs);
			
			if (packet->type == Player_Delta_State || packet->type == Player_Full_State) {
				int peer = packet->type == Player_Delta_State ? ((PlayerDeltaPacket*)packet)->playerID : ((PlayerFullPacket*)packet)->playerID;
				manager->UpdateServerPlayer(peer, packet);

				
			}

			if (packet->type == Player_Finished) manager->UpdateServerPlayer(((PlayerFinishedPacket*)packet)->playerID, packet);
			
			if(packet->type == Player_State) manager->UpdateServerPlayer(((PlayerFinishedPacket*)packet)->playerID, packet);

			if (packet->type == Exit_Lobby) manager->ActivateExitLobby();

		}
		enet_packet_destroy(event.packet);
	}
}

void NCL::CSC8508::GameClient::UpdateClientLobby(int* playerIDs)
{
	while (!manager->GetPlayerLobby()->empty()) //empty queue
				manager->GetPlayerLobby()->pop();
	for (int i = 0; i < 8; i++) {
		int id = *(playerIDs + i);
		if (id == -1) continue;

		manager->AddPlayerToLobby(id);
	}
}

void GameClient::SendPacket(GamePacket&  payload) {
	ENetPacket* dataPacket = enet_packet_create(&payload, payload.GetTotalSize(), 0);

	int test = enet_peer_send(netPeer, 0, dataPacket);
}

//void GameClient::ThreadedUpdate() {
//	while (threadAlive) {
//		UpdateClient();
//	}
//}

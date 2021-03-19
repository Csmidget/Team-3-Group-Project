#include "NetworkManager.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC8508;

#define COLLISION_MSG 30

struct MessagePacket : public GamePacket {
	short playerID;
	short messageID;

	MessagePacket() {
		type = Message;
		size = sizeof(short) * 2;
	}
};


NetworkManager::NetworkManager()
{
	if (OFFLINE_MODE) return;
	
	NetworkBase::Initialise();
	isClient ? StartAsClient() : StartAsServer();
	
}

NetworkManager::NetworkManager(bool client) {

	NetworkBase::Initialise();
	isClient = client;
	client ? StartAsClient() : StartAsServer();
}

NetworkManager::~NetworkManager()
{
	NetworkBase::Destroy();
	delete thisServer;
	delete thisClient;
}


void NetworkManager::Update(float dt)
{
	if (OFFLINE_MODE) return;
	timeToNextPacket -= dt;

	if (timeToNextPacket < 0) {
		isClient ? UpdateAsClient(dt) : UpdateAsServer(dt);	
		timeToNextPacket += 1.0f / 20.0f; //20hz server/client update
	}

	

	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::R)) Restart();
}

void NCL::CSC8508::NetworkManager::ReceivePacket(int type, GamePacket* payload, int source)
{
	std::cout << "SOME SORT OF PACKET RECEIVED" << std::endl;
	if (type == Received_State) {	//Server version of the game receives these from players
		ClientPacket* realPacket = (ClientPacket*)payload;
	}
	//CLIENT version of the game will receive these from the servers
	
	if (type == Player_Delta_State) {
		PlayerDeltaPacket* realPacket = (PlayerDeltaPacket*)payload;
		if (realPacket->playerID < (int)serverPlayers.size()) {
			serverPlayers[realPacket->playerID]->ReadPacket(*realPacket);
		}
	}
	else if (type == Player_Full_State) {
		PlayerFullPacket* realPacket = (PlayerFullPacket*)payload;
		if (realPacket->playerID < (int)serverPlayers.size()) {
			serverPlayers[realPacket->playerID]->ReadPacket(*realPacket);
		}
	}
	else if (type == Message) {
		MessagePacket* realPacket = (MessagePacket*)payload;

		if (realPacket->messageID == COLLISION_MSG) {
			std::cout << "Client: Received collision message!" << std::endl;
		}
	}
	else if (type == Player_Connected) {
		NewPlayerPacket* realPacket = (NewPlayerPacket*)payload;
		std::cout << "Client: New player connected!" << std::endl;
	}
	else if (type == Player_Disconnected) {
		PlayerDisconnectPacket* realPacket = (PlayerDisconnectPacket*)payload;
		std::cout << "Client: Player Disconnected!" << std::endl;
	}
}

void NCL::CSC8508::NetworkManager::AddPlayerToLobby(int id)
{
	if (isClient) 
		if (thisClient->GetID() == id) return;
	

	playerLobby.emplace(id);
}





void NCL::CSC8508::NetworkManager::SetLocalPlayer(GameObject* object, bool* isLocalFinished, int* localScore)
{
	ClientPlayer* player = new ClientPlayer("Me", *object, thisClient ? thisClient->GetID() : 0);
	localPlayer = new LocalPlayer(player, isLocalFinished, localScore);

}

void NCL::CSC8508::NetworkManager::UpdateServerPlayer(int id, GamePacket* packet)
{
	std::map<int, ClientPlayer*>::iterator it;
	it = serverPlayers.find(id);
	if (it == serverPlayers.end()) return;

	ClientPlayer* player = it->second;
	if (player && packet) player->Update(*packet);
}

void NCL::CSC8508::NetworkManager::StartAsServer()
{
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), MAX_CLIENTS,  this);
	thisServer->RegisterPacketHandler(Received_State, this);
}

void NCL::CSC8508::NetworkManager::StartAsClient()
{
	thisClient = new GameClient(this);

	thisClient->Connect(80, 5, 123, 22, NetworkBase::GetDefaultPort());
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);
	thisClient->RegisterPacketHandler(Player_Count, this);

}

void NetworkManager::UpdateAsServer(float dt)
{
	if (!thisServer) return;
	thisServer->UpdateServer();
	

	packetsToSnapshot--;
	if (packetsToSnapshot < 0) {
		BroadcastSnapshot(false);
		packetsToSnapshot = 5;
	}
	else {
		BroadcastSnapshot(true);
	}
}

void NetworkManager::UpdateAsClient(float dt)
{
	if (!thisClient) return;
	thisClient->UpdateClient();
	
	GamePacket* newPacket;

	if (!localPlayer) return;
	localPlayer->player->WritePacket(&newPacket, dt, stateID);
	thisClient->SendPacket(*newPacket);
	
	//stateID++;
	delete newPacket;
}

void NCL::CSC8508::NetworkManager::BroadcastSnapshot(bool deltaFrame)
{
	for (int i = 0; i < serverPlayers.size(); i++) {
		GamePacket* newPacket;
		serverPlayers.at(i)->WritePacket(&newPacket, deltaFrame, stateID);
		thisServer->SendGlobalPacket(*newPacket);
		delete newPacket;
		

	}
	//stateID++;
	//std::vector<GameObject*>::const_iterator first;
	//std::vector<GameObject*>::const_iterator last;

	//world->GetObjectIterators(first, last);

	//for (auto i = first; i != last; ++i) {
		
		//NetworkObject* o = (*i)->GetNetworkObject();
		//if (!o) {
		//	continue;
		//}
		////TODO - you'll need some way of determining
		////when a player has sent the server an acknowledgement
		////and store the lastID somewhere. A map between player
		////and an int could work, or it could be part of a 
		////NetworkPlayer struct. 
		//int playerState = 0;
		//GamePacket* newPacket = nullptr;
		//if (o->WritePacket(&newPacket, deltaFrame, playerState)) {
		//	thisServer->SendGlobalPacket(*newPacket);
		//	delete newPacket;
		//}
	//}
}

void NCL::CSC8508::NetworkManager::UpdateMinimumState()
{
	////Periodically remove old data from the server
	//int minID = INT_MAX;
	//int maxID = 0; //we could use this to see if a player is lagging behind?

	//for (auto i : stateIDs) {
	//	minID = min(minID, i.second);
	//	maxID = max(maxID, i.second);
	//}
	////every client has acknowledged reaching at least state minID
	////so we can get rid of any old states!
	//std::vector<GameObject*>::const_iterator first;
	//std::vector<GameObject*>::const_iterator last;
	//world->GetObjectIterators(first, last);

	//for (auto i = first; i != last; ++i) {
	//	NetworkObject* o = (*i)->GetNetworkObject();
	//	if (!o) {
	//		continue;
	//	}
	//	o->UpdateStateHistory(minID); //clear out old states so they arent taking up memory...
	//}
}

void NCL::CSC8508::NetworkManager::Restart()
{
	NetworkBase::Destroy();
	delete thisServer;
	delete thisClient;
	NetworkBase::Initialise();
	isClient ? StartAsClient() : StartAsServer();
}



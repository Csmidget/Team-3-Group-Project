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
	if(TEST_MODE) isClient ? TestClient() : TestServer();

	isClient ? StartAsClient() : StartAsServer();
	
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

}

void NCL::CSC8508::NetworkManager::ReceivePacket(int type, GamePacket* payload, int source)
{
	std::cout << "SOME SORT OF PACKET RECEIVED" << std::endl;
	if (type == Received_State) {	//Server version of the game receives these from players
		ClientPacket* realPacket = (ClientPacket*)payload;
	}
	//CLIENT version of the game will receive these from the servers
	else if (type == Delta_State) {
		DeltaPacket* realPacket = (DeltaPacket*)payload;
		if (realPacket->objectID < (int)networkObjects.size()) {
			networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
		}
	}
	else if (type == Full_State) {
		FullPacket* realPacket = (FullPacket*)payload;
		if (realPacket->objectID < (int)networkObjects.size()) {
			networkObjects[realPacket->objectID]->ReadPacket(*realPacket);
		}
	}
	else if (type == Player_Delta_State) {
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
	playerLobby.emplace(id);
}

//void NCL::CSC8508::NetworkManager::OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b)
//{
//	if (thisServer) { //detected a collision between players!
//		MessagePacket newPacket;
//		newPacket.messageID = COLLISION_MSG;
//		newPacket.playerID = a->GetPlayerNum();
//
//		thisClient->SendPacket(newPacket);
//
//		newPacket.playerID = b->GetPlayerNum();
//		thisClient->SendPacket(newPacket);
//	}
//}

void NetworkManager::TestClient()
{
	string clientName = "Client1";
	
	TestPacketReceiver clientReceiver(clientName);
	GameClient* client = new GameClient();
	client->RegisterPacketHandler(String_Message, &clientReceiver);
	bool canConnect = client->Connect(80, 5, 123, 22, NetworkBase::GetDefaultPort());

	int i = 0;
	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		StringPacket strPacket(clientName + " says hello! " + to_string(i));
		client->SendPacket(strPacket);
		i++;
		client->UpdateClient();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void NetworkManager::TestServer()
{
	TestPacketReceiver serverReceiver("Server");
	GameServer* server = new GameServer(NetworkBase::GetDefaultPort(), 8, this);
	server->RegisterPacketHandler(String_Message, &serverReceiver);

	while (!Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		StringPacket strPacket("Server says hello! ");
		server->SendGlobalPacket(strPacket);
		server->UpdateServer();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
}

void NCL::CSC8508::NetworkManager::StartAsServer()
{
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), 4,  this);
	thisServer->RegisterPacketHandler(Received_State, this);
}

void NCL::CSC8508::NetworkManager::StartAsClient()
{
	thisClient = new GameClient();
	thisClient->Connect(80, 5, 123, 22, NetworkBase::GetDefaultPort());
	thisClient->RegisterPacketHandler(Delta_State, this);
	thisClient->RegisterPacketHandler(Full_State, this);
	thisClient->RegisterPacketHandler(Player_Connected, this);
	thisClient->RegisterPacketHandler(Player_Disconnected, this);

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

//	ClientPacket newPacket;
/*
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		//fire button pressed!
		newPacket.buttonstates[0] = 1;
		newPacket.lastID = 0; //You'll need to work this out somehow...
	}*/
	GamePacket* newPacket;
	localPlayer->WritePacket(&newPacket, dt, stateID);
	thisClient->SendPacket(*newPacket);
}

void NCL::CSC8508::NetworkManager::BroadcastSnapshot(bool deltaFrame)
{
	std::vector<GameObject*>::const_iterator first;
	std::vector<GameObject*>::const_iterator last;

	//world->GetObjectIterators(first, last);

	for (auto i = first; i != last; ++i) {
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
	}
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



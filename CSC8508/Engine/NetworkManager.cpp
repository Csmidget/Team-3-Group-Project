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
}

void NCL::CSC8508::NetworkManager::ReceivePacket(int type, GamePacket* payload, int source)
{
	//std::cout << "SOME SORT OF PACKET RECEIVED" << std::endl;
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





void NCL::CSC8508::NetworkManager::SetLocalPlayer(GameObject* object)
{
	ClientPlayer* player = new ClientPlayer("Me", *object, thisClient ? thisClient->GetID() : 0);
	localPlayer = new LocalPlayer(player);

}

void NCL::CSC8508::NetworkManager::UpdateServerPlayer(int id, GamePacket* packet)
{
	std::map<int, ClientPlayer*>::iterator it;
	it = serverPlayers.find(id);
	if (it == serverPlayers.end()) return;

	ClientPlayer* player = it->second;
	if (player && packet) player->Update(*packet);
}

bool NCL::CSC8508::NetworkManager::IsExitLobbyTime()
{
	if (!exitLobby) return false;
	exitLobby = false;
	return true;
}

bool NCL::CSC8508::NetworkManager::IsAllPlayersFinished()
{
	NetworkPlayerComponent* npc;
	for (auto i = serverPlayers.begin(); i != serverPlayers.end(); ++i) {
		npc = i->second->GetNetworkPlayerComponent();
		if (!npc->isFinished()) return false;

	}

	if (isClient) 
		if (!localPlayer->isFinished) return false;
	

	std::cout << "All Players Finished " << std::endl;
	return true;
}

bool NCL::CSC8508::NetworkManager::IsMajorityPlayersFinished(float percentage) const
{
	int playersFinished = GetNoOfPlayersFinished();
	int totalPlayers = serverPlayers.size() + (isClient ? 1 : 0);

	return playersFinished >= std::ceil((float)totalPlayers * percentage);
}

void NCL::CSC8508::NetworkManager::StartAsServer()
{
	thisServer = new GameServer(NetworkBase::GetDefaultPort(), MAX_CLIENTS,  this);
	thisServer->RegisterPacketHandler(Received_State, this);
	thisServer->RegisterPacketHandler(Player_Finished, this);
	thisServer->RegisterPacketHandler(Player_State, this);
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
	thisClient->RegisterPacketHandler(Player_Finished, this);
	thisClient->RegisterPacketHandler(Exit_Lobby, this);
	thisClient->RegisterPacketHandler(Player_State, this);

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

	if (IsExitLobbyTime()) {
		GamePacket* packet= new ExitLobbyPacket();
		thisServer->SendGlobalPacket(*packet);
		delete packet;
	} 
}

void NetworkManager::UpdateAsClient(float dt)
{
	if (!thisClient) return;
	thisClient->UpdateClient();
	
	GamePacket* newPacket;

	if (!localPlayer) return;
	UpdateLocalPlayer(dt);
}

void NCL::CSC8508::NetworkManager::UpdateLocalPlayer(float dt)
{
	GamePacket* newPacket;

	localPlayer->player->WritePacket(&newPacket, dt, stateID);
	thisClient->SendPacket(*newPacket);
	delete newPacket;

	newPacket = new PlayerStatusPacket(localPlayer->player->GetPlayerID(), localPlayer->score, localPlayer->isFinished);
	thisClient->SendPacket(*newPacket);
	delete newPacket;

}

void NCL::CSC8508::NetworkManager::BroadcastSnapshot(bool deltaFrame)
{
	for (int i = 0; i < serverPlayers.size(); i++) {
		GamePacket* newPacket;
		serverPlayers.at(i)->WritePacket(&newPacket, deltaFrame, stateID);
		thisServer->SendGlobalPacket(*newPacket);
		delete newPacket;

		NetworkPlayerComponent* player = serverPlayers.at(i)->GetNetworkPlayerComponent();

		if (player) {
			newPacket = new PlayerStatusPacket(serverPlayers.at(i)->GetPlayerID(), player->GetScore(), player->isFinished());
			thisServer->SendGlobalPacket(*newPacket);
			delete newPacket;
		}
	}

}



void NCL::CSC8508::NetworkManager::Restart()
{
	NetworkBase::Destroy();
	delete thisServer;
	delete thisClient;
	NetworkBase::Initialise();
	isClient ? StartAsClient() : StartAsServer();
}

int NCL::CSC8508::NetworkManager::GetNoOfPlayersFinished() const
{
	int noOfPlayers = 0;
	NetworkPlayerComponent* npc;
	for (auto i = serverPlayers.begin(); i != serverPlayers.end(); ++i) {
		npc = i->second->GetNetworkPlayerComponent();
		if (npc->isFinished()) noOfPlayers++;
	}

	if (isClient)
		if (localPlayer->isFinished) noOfPlayers++;

	return noOfPlayers;
}



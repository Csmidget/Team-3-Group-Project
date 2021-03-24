#pragma once
#include "../Engine/GameServer.h"
#include "../Engine/GameClient.h"
#include "../Engine/NetworkObject.h"
#include "../Engine/NetworkBase.h"
#include "../Engine/ClientPlayer.h"

using namespace std;

#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <queue>

class TestPacketReceiver : public PacketReceiver {
public:
	TestPacketReceiver(string name) {
		this->name = name;
	}
	void ReceivePacket(int type, GamePacket* payload, int source) {
		if (type == String_Message) {
			StringPacket* realPacket = (StringPacket*)payload;

			string msg = realPacket->GetStringFromData();

			cout << name << " received message: " << msg << endl;
		}
	}
	string name;
};
namespace NCL {
	namespace CSC8508 {

		struct LocalPlayer {
			LocalPlayer(ClientPlayer* localPlayer) {
				player = localPlayer;
				isFinished = false;
				score= 0;
			}
			ClientPlayer* player;
			bool isFinished;
			int score;

		};

		class NetworkManager : public PacketReceiver
		{
		public:
			NetworkManager();
			NetworkManager(bool client);

			~NetworkManager();

			void Update(float dt);

			void ReceivePacket(int type, GamePacket* payload, int source) override;
			//void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);
	
			void AddPlayerToLobby(int id);

			std::queue<int>* GetPlayerLobby() {
				return &playerLobby;
			}

			void AddPlayerToGame(int id, GameObject* object) {
				serverPlayers.emplace(id, new ClientPlayer("Client", *object, id));
			}
			
			void SetLocalPlayer(GameObject* object); //{ localPlayer = new ClientPlayer("Me",*object, thisClient ? thisClient->GetID() : 0); }
			LocalPlayer* GetLocalPlayer() { return localPlayer; }

			void UpdateServerPlayer(int id, GamePacket* packet);

			bool IsExitLobbyTime();
			bool IsClient() const { return isClient;  }
			void ActivateExitLobby() { exitLobby = true; }

			bool IsAllPlayersFinished();
			bool IsMajorityPlayersFinished(float percentage = 0.6f) const;
		private:
		

			void StartAsServer();
			void StartAsClient();

			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void UpdateLocalPlayer(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();

			void Restart();
			int GetNoOfPlayersFinished() const;

			bool const OFFLINE_MODE = false;
			bool isClient = false;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			bool exitLobby = false;

			std::map<int, ClientPlayer*> serverPlayers;
			
//			ClientPlayer* localPlayer;
//			GameStateManagerComponent* gameState;

			LocalPlayer* localPlayer;

			std::queue<int> playerLobby;

			int stateID = 0;

			const int MAX_CLIENTS = 8;

		};

	}
}
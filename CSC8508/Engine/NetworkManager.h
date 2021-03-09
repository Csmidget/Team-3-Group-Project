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
//#include "../Game/NetworkPlayer.h"

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
		class NetworkManager : public PacketReceiver
		{

		public:
			NetworkManager();
			~NetworkManager();

			void Update(float dt);

			void ReceivePacket(int type, GamePacket* payload, int source) override;
			//void OnPlayerCollision(NetworkPlayer* a, NetworkPlayer* b);

		private:
			void TestClient();
			void TestServer();

			void StartAsServer();
			void StartAsClient();

			void UpdateAsServer(float dt);
			void UpdateAsClient(float dt);

			void BroadcastSnapshot(bool deltaFrame);
			void UpdateMinimumState();


			bool const OFFLINE_MODE = true;
			bool const TEST_MODE = true;
			bool isClient = true;

			GameServer* thisServer;
			GameClient* thisClient;
			float timeToNextPacket;
			int packetsToSnapshot;

			std::vector<NetworkObject*> networkObjects;

			std::map<int, ClientPlayer*> serverPlayers;
			GameObject* localPlayer;
		};

	}
}
#pragma once
#include "../Engine/GameServer.h"
#include "../Engine/GameClient.h"
#include "../Engine/NetworkObject.h"
#include "../Engine/NetworkBase.h"

using namespace NCL;
using namespace CSC8508;
using namespace std;

#include <chrono>
#include <thread>
#include <string>
#include <iostream>

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

class NetworkManager
{

	public:
		NetworkManager();
		~NetworkManager();
		void Update(float dt);
	private:
		void TestClient();
		void TestServer();

		bool const OFFLINE_MODE = false;
		bool isClient = false;
};


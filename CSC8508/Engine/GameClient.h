#pragma once
#include "NetworkBase.h"
#include <stdint.h>
#include <thread>
#include <atomic>

namespace NCL {
	namespace CSC8508 {
		class GameObject;
		class NetworkManager;
		class GameClient : public NetworkBase {
		public:
			GameClient(NetworkManager* manager);
			~GameClient();

			bool Connect(uint8_t a, uint8_t b, uint8_t c, uint8_t d, int portNum);

			void SendPacket(GamePacket&  payload);

			void UpdateClient();

			int GetID() const { return id; }
		protected:	
			//void ThreadedUpdate();
			NetworkManager* manager;

			ENetPeer*	netPeer;
			int id = -1;
			
			void UpdateClientLobby(int* playerIDs);
			//std::atomic<bool>	threadAlive;
			//std::thread			updateThread;
		};
	}
}


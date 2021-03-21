#pragma once
#include <thread>
#include <atomic>

#include "NetworkBase.h"

namespace NCL {
	namespace CSC8508 {
		class NetworkManager;
		class GameServer : public NetworkBase {
		public:
			GameServer(int onPort, int maxClients, NetworkManager* manager);
			~GameServer();

			bool Initialise();
			void Shutdown();


			//void ThreadedUpdate();

			bool SendGlobalPacket(int msgID);
			bool SendGlobalPacket(GamePacket& packet);

			virtual void UpdateServer();

		protected:
			int			port;
			int			clientMax;
			int			clientCount;
			NetworkManager* manager;

			//std::atomic<bool> threadAlive;

			

			//std::thread updateThread;

			int incomingDataRate;
			int outgoingDataRate;

			void SendGlobalLobby();

		};
	}
}

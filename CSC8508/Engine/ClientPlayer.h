#pragma once
#include "NetworkObject.h"
#include <string>


//The players of the other clients
namespace NCL {
	namespace CSC8508 {
		class ClientPlayer : public NetworkObject
		{
		public:

			ClientPlayer(std::string clientName, GameObject& o, int id);

			void Update(GamePacket& p) override;
		protected:

			bool WriteDeltaPacket(GamePacket** p, int stateID) override;
			bool WriteFullPacket(GamePacket** p) override;

			int playerID;
			std::string clientName;
		};
	}
}
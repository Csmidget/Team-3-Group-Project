#pragma once
#include "GameObject.h"
#include "NetworkBase.h"
#include "NetworkState.h"
namespace NCL {
	namespace CSC8508 {
		struct FullPacket : public GamePacket {
			int		objectID = -1;
			NetworkState fullState;

			FullPacket() {
				type = Full_State;
				size = sizeof(FullPacket) - sizeof(GamePacket);
			}
		};

		struct DeltaPacket : public GamePacket {
			int		fullID		= -1;
			int		objectID	= -1;
			char	pos[3];
			char	orientation[4];

			DeltaPacket() {
				type = Delta_State;
				size = sizeof(DeltaPacket) - sizeof(GamePacket);
			}
		};

		struct PlayerFullPacket : public FullPacket {
			int		playerID = -1;
			//NetworkState fullState;

			PlayerFullPacket() {
				type = Player_Full_State;
				size = sizeof(PlayerFullPacket) - sizeof(GamePacket);
			}
		};

		struct PlayerDeltaPacket : public DeltaPacket {
			//int		fullID = -1;
			int		playerID = -1;
			//char	pos[3];
			//char	orientation[4];

			PlayerDeltaPacket() {
				type = Player_Delta_State;
				size = sizeof(PlayerDeltaPacket) - sizeof(GamePacket);
			}
		};
		struct ClientPacket : public GamePacket {
			int		lastID;
			char	buttonstates[10];

			ClientPacket() {
				size = sizeof(ClientPacket);
			}
		};
	


		class NetworkObject
		{
		public:
			NetworkObject(GameObject& o, int id);
			virtual ~NetworkObject();

			//Called by clients
			virtual bool ReadPacket(GamePacket& p);
			//Called by servers
			virtual bool WritePacket(GamePacket** p, bool deltaFrame, int stateID);

			virtual void Update(GamePacket& p);
			void UpdateStateHistory(int minID);

		protected:

			NetworkState& GetLatestNetworkState();

			bool GetNetworkState(int frameID, NetworkState& state);

			virtual bool ReadDeltaPacket(DeltaPacket &p);
			virtual bool ReadFullPacket(FullPacket &p);

			virtual bool ReadPlayerFinishedPacket(PlayerFinishedPacket& p);
			virtual bool ReadPlayerStatusPacket(PlayerStatusPacket& p) { return false; };
			virtual bool WriteDeltaPacket(GamePacket**p, int stateID);
			virtual bool WriteFullPacket(GamePacket**p);


			NetworkState lastFullState;

			std::vector<NetworkState> stateHistory;

			int deltaErrors;
			int fullErrors;

			GameObject& object;
			Quaternion orientation;

			int networkID;
		};
	}
}


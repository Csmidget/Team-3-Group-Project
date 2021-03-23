#include "ClientPlayer.h"

using namespace NCL;
using namespace CSC8508;

NCL::CSC8508::ClientPlayer::ClientPlayer(std::string clientName, GameObject& o, int id) : NetworkObject(o, id) 
{
	this->clientName = clientName;
	playerID = id;
}

void NCL::CSC8508::ClientPlayer::Update(GamePacket& p)
{
	ReadPacket(p);
}




bool NCL::CSC8508::ClientPlayer::ReadPlayerFinishedPacket(PlayerFinishedPacket& p)
{
	if (p.playerID == this->playerID) {
		NetworkPlayerComponent* player = GetNetworkPlayerComponent();
		if (player->isFinished()) return true;
		player->SetScore(p.score);
		player->Finish();
		std::cout << "Player " << p.playerID << " has finished the level and scored " << p.score << std::endl;
	}


	return true;
}

bool NCL::CSC8508::ClientPlayer::ReadPlayerStatusPacket(PlayerStatusPacket& p)
{
	if (p.playerID == this->playerID) {
		NetworkPlayerComponent* player = GetNetworkPlayerComponent();
		player->SetScore(p.score);
		if (p.isFinished) player->Finish();
	}
	return true;
}

bool NCL::CSC8508::ClientPlayer::WriteDeltaPacket(GamePacket** p, int stateID)
{
	PlayerDeltaPacket* dp = new PlayerDeltaPacket();

	dp->playerID = this->playerID;
	dp->objectID = networkID;

	NetworkState state;
	if (!GetNetworkState(stateID, state)) {
		return false; //can't delta!
	}

	dp->fullID = stateID;

	Vector3		currentPos = object.GetTransform().GetPosition();
	Quaternion  currentOrientation = object.GetTransform().GetOrientation();

	currentPos -= state.position;
	currentOrientation -= state.orientation;

	dp->pos[0] = (char)currentPos.x;
	dp->pos[1] = (char)currentPos.y;
	dp->pos[2] = (char)currentPos.z;

	dp->orientation[0] = (char)(currentOrientation.x * 127.0f);
	dp->orientation[1] = (char)(currentOrientation.y * 127.0f);
	dp->orientation[2] = (char)(currentOrientation.z * 127.0f);
	dp->orientation[3] = (char)(currentOrientation.w * 127.0f);

	*p = dp;
	return true;
}

bool NCL::CSC8508::ClientPlayer::WriteFullPacket(GamePacket** p)
{
	PlayerFullPacket* fp = new PlayerFullPacket();

	fp->playerID = this->playerID;
	fp->objectID = networkID;
	fp->fullState.position = object.GetTransform().GetPosition();
	fp->fullState.orientation = object.GetTransform().GetOrientation();
	fp->fullState.stateID = lastFullState.stateID++;

	*p = fp;
	return true;
}


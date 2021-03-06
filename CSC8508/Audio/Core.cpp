#include "Core.h"


using namespace NCL::CSC8508;
int Audio::ErrorCheck(FMOD_RESULT result)
{
	{
		if (result == FMOD_OK)
			return 0;

		std::cout << "FMOD ERROR: " << result << std::endl;
		return 1;
	}
}

FMOD_VECTOR Audio::ToFMODVECTOR(const NCL::Maths::Vector3& v)
{
	FMOD_VECTOR vec;
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;
	return vec;
}

using namespace NCL::CSC8508::Audio;
Core::Core():
	coreSystem(nullptr), coreNextChannelID(0)
{
	Audio::ErrorCheck(FMOD::System_Create(&coreSystem));
	Audio::ErrorCheck(coreSystem->init(512, FMOD_INIT_NORMAL, NULL));
}

Core::~Core()
{
	Audio::ErrorCheck(coreSystem->release());
}

void Core::Update()
{
	std::vector<ChannelMap::iterator> stoppedChannels;

	for (auto it = coreChannels.begin(); it != coreChannels.end(); ++it)
	{
		bool isChannelPlaying = false;
		it->second->isPlaying(&isChannelPlaying);
		if (!isChannelPlaying)
			stoppedChannels.push_back(it);
	}
	//Clears stopped channel vector
	for (auto& it : stoppedChannels)
		coreChannels.erase(it);

	//Clears deleted SoundInstances
	auto it = coreSounds.begin();
	while (it != coreSounds.end())
	{
		if (it->second.size() == 0)
			it = coreSounds.erase(it);
		else
			it++;
	}

	//Clear release one shot sound
	auto shotsIt = oneShots.begin();
	while (shotsIt != oneShots.end())
	{
		auto playing = false;
		shotsIt->first->isPlaying(&playing);
		if (!playing)
		{
			shotsIt->second->release();
			shotsIt = oneShots.erase(shotsIt);
		}
		else
			shotsIt++;
	}
	Audio::ErrorCheck(coreSystem->update());
}
#include "Core.h"

using namespace NCL::CSC8508::Audio;

int NCL::CSC8508::Audio::ErrorCheck(FMOD_RESULT result)
{
	{
		if (result == FMOD_OK)
			return 0;

		std::cout << "FMOD ERROR: " << result << std::endl;
		return 1;
	}
}


Core::Core():
	coreSystem(nullptr), coreNextChannelID(0)
{
	ErrorCheck(FMOD::System_Create(&coreSystem));
	ErrorCheck(coreSystem->init(512, FMOD_INIT_NORMAL, NULL));
}

Core::~Core()
{
	ErrorCheck(coreSystem->release());
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
		if ((*it).second.size() == 0)
			it = coreSounds.erase(it);
		else
			it++;
	}
	ErrorCheck(coreSystem->update());
}
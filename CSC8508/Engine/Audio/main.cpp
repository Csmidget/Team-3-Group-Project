#include"../Audio/External/inc/fmod.hpp"


int main()
{
	FMOD_RESULT result;
	FMOD::System* system = nullptr;
	result = FMOD::System_Create(&system);
	result = system->init(512, FMOD_INIT_NORMAL, 0);

	FMOD::Sound* sound;

	system->createSound("res/Dogs_bark_long.wav", FMOD_DEFAULT, 0, &sound);
	system->playSound(sound, 0, false, 0);

	while (true);




}
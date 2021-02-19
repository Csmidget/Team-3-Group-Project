#include"../Audio/External/inc/fmod.hpp"
#include"SoundInstance.h";

using namespace NCL::CSC8508;
int main()
{

	//FMOD_RESULT result;
	//FMOD::System* system = nullptr;
	//result = FMOD::System_Create(&system);
	//result = system->init(512, FMOD_INIT_NORMAL, 0);

	//FMOD::Sound* sound;

	//system->createSound("res/Dogs_bark_long.wav", FMOD_DEFAULT, 0, &sound);
	//system->playSound(sound, 0, false, 0);
	//while (true);
	Audio::SoundManager::Init();
	Audio::SoundInstance test;
	Audio::SoundManager::CreateInstance("res/Dogs_bark_long.wav", &test);
	test.Play();
    Audio::SoundManager::Update();
	while (true);
 }
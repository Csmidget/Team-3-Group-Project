#include"../Audio/External/inc/fmod.hpp"
#include"SoundManager.h";

using namespace NCL::CSC8508;
int main()
{
	Audio::SoundManager::Init();

	Audio::SoundInstance* test = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance("res/Dogs_bark_long.wav", test);
	test->Play();

	Audio::SoundInstance* test1 = new Audio::SoundInstance();;
	Audio::SoundManager::CreateInstance("res/River.mp3", test1);

	test1->SetMode(Audio::MODE_3D | Audio::MODE_LOOP_ON);
	test1->SetPitch(0.5f);
	test1->SetVolume(1.0f);
	test1->Play();
	bool is = test1->isPaused();
	test1->SetPaused(true);
	is = test1->isPaused();

	Audio::SoundInstance* test2 = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance("res/Dogs_bark_long.wav", test2);
	//test2->Play();

	delete test2;
	delete test;
    Audio::SoundManager::Update();
	test1->SetPaused(false);

	while (true)
		std::cout << test1->isPlaying() << std::endl;
	
 }
#pragma once
#include"../Audio/External/inc/fmod.hpp"
#include"SoundManager.h";

#include"../../../Common/Vector3.h"


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
	//test1->SetPitch(2.0f);
	test1->SetVolume(1.0f);
	test1->Set3DAttributes(NCL::Maths::Vector3(0, 0, 0), NCL::Maths::Vector3(0, 0, 0));
	Audio::SoundManager::SetListenerParameters(NCL::Maths::Vector3(1, 0, 0));
	test1->Play();
	bool is = test1->isPaused();
	test1->SetPaused(true);
	is = test1->isPaused();

	Audio::SoundInstance* test2 = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance("res/Dogs_bark_long.wav", test2);
	//test2->Play();

	delete test2;
	
    Audio::SoundManager::Update();
	test1->SetPaused(false);
	
	float i = 0;
	while (true)
	{
		Audio::SoundManager::SetListenerParameters(NCL::Maths::Vector3(i, 0, 0));
		i+=0.001f;
		Audio::SoundManager::Update();
		std::cout << i << std::endl;
	}
	
 }
#include "SoundManager.h"

SoundManager::SoundManager()
{
	musicEngine = irrklang::createIrrKlangDevice();
}

SoundManager::~SoundManager()
{
	if (BGM)
		BGM->drop();

	if (SoundEffect2D)
		SoundEffect2D->drop();

	if (musicEngine)
		musicEngine->drop();
}

void SoundManager::playMusic(string Music)
{
	BGM = musicEngine->play2D(Music.c_str(), true, false, true);
	//BGM->setVolume(0.5);
}

void SoundManager::stopMusic()
{
	BGM->stop();
}

void SoundManager::playSoundEffect2D(string Music)
{
	SoundEffect2D = musicEngine->play2D(Music.c_str(), false, false, false);
}
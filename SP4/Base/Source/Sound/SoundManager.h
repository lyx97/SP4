#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

//#pragma comment(lib, "irrKlang.lib")

#include "SingletonTemplate.h"
#include <irrKlang.h>
#include <string>
using std::string;

class SoundManager : public Singleton<SoundManager>
{
	friend Singleton<SoundManager>;

public:
	SoundManager(void);
	virtual ~SoundManager(void);

	void playMusic(string Music);
	void stopMusic();
	void playSoundEffect2D(string Music);

private:
	irrklang::ISoundEngine* musicEngine;
	irrklang::ISound* BGM;
	irrklang::ISound* SoundEffect2D;
};

#endif
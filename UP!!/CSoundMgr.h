#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <cassert>
#include <array>

#include "Define.h"

class CSoundMgr
{
private:
	static CSoundMgr* instance;
	CSoundMgr();
	~CSoundMgr();

public:
	static CSoundMgr* getInst();


private:
	FMOD::System* system;
	FMOD::Sound* bgm;
	FMOD::Channel* channels[2] = {0, 0};
	FMOD_RESULT result;
	void* extradriverdata = 0;
	std::array<FMOD::Sound*, SOUND_MAX> sounds;

public:
	void playSound(const int _idx);

};


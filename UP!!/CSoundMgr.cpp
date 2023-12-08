#include "CSoundMgr.h"

CSoundMgr* CSoundMgr::instance = nullptr;

CSoundMgr::CSoundMgr()
{

	result = FMOD::System_Create(&system);
	if (result != FMOD_OK) {
		assert(false);
	}

	system->init(32, FMOD_INIT_NORMAL, extradriverdata);
	system->createSound("sound\\background.mp3", FMOD_LOOP_NORMAL, 0, &bgm);

	for (int i = 0; i < SOUND_MAX; ++i) {
		system->createSound(SOUND_NAME[i], FMOD_LOOP_OFF, 0, &sounds[i]);
	}

	system->playSound(bgm, 0, false, &channels[0]);
	channels[0]->setVolume(0.3);
}

CSoundMgr* CSoundMgr::getInst()
{
	if (!instance) {
		instance = new CSoundMgr();
	}
	return instance;
}

void CSoundMgr::playSound(const int _idx)
{
	system->playSound(sounds[_idx], 0, false, &channels[1]);
}

CSoundMgr::~CSoundMgr()
{
	if (instance != nullptr)
		delete instance;
}
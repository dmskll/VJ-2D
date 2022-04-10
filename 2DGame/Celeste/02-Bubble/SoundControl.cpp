#include "SoundControl.h"

SoundControl::SoundControl() {
	engine = NULL;
}

SoundControl::~SoundControl() {
	engine->drop();
}

void SoundControl::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* SoundControl::getSoundEngine() {
	return engine;
}
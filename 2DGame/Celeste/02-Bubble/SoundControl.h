#ifndef _SOUND_CONTROL_INCLUDE
#define _SOUND_CONTROL_INCLUDE

#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

class SoundControl {

public:
	SoundControl();
	~SoundControl();
	static SoundControl& instance() {
		static SoundControl SM;
		return SM;
	}

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
};

#endif // _SOUND_MANAGER_INCLUDE

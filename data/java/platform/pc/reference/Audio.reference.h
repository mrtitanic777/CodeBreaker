// paulscode SoundSystem on the PC: sources play the game's own .ogg/.wav files (libvorbisfile), mixed on
// SDL2's audio device. Sound effects are decoded once and cached; music and streaming sources decode as
// they play. Positional sources fade linearly with distance (paulscode's ATTENUATION_LINEAR) and pan by
// the listener's orientation. Every call is thread-safe (the mixer runs on SDL's audio thread).
#ifndef AUDIO_H
#define AUDIO_H
#include <string>
#include "URL.h"

class SoundSystem {
public:
	SoundSystem(...);
	virtual ~SoundSystem();
	void cleanup();
	void newSource(bool priority, const std::string& source, URL* url, const std::string& identifier,
	               bool loop, float x, float y, float z, int attModel, float dist);
	void newStreamingSource(bool priority, const std::string& source, URL* url, const std::string& identifier,
	                        bool loop, float x, float y, float z, int attModel, float dist);
	void backgroundMusic(const std::string& source, URL* url, const std::string& identifier, bool loop);
	void quickPlay(bool priority, URL* url, const std::string& identifier, bool loop,
	               float x, float y, float z, int attModel, float dist);
	void play(const std::string& source);
	void stop(const std::string& source);
	void pause(const std::string& source);
	void removeSource(const std::string& source);
	void setVolume(const std::string& source, float volume);
	void setPitch(const std::string& source, float pitch);
	void setPosition(const std::string& source, float x, float y, float z);
	void setListenerPosition(float x, float y, float z);
	void setListenerOrientation(float lx, float ly, float lz, float ux, float uy, float uz);
	bool playing(const std::string& source);
	void setMasterVolume(float v);
	inline void fadeOut(...) {}
	inline void fadeOutIn(...) {}
	inline void quickStream(...) {}
};
#endif // AUDIO_H

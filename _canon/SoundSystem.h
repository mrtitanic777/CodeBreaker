#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for paulscode.sound.SoundSystem.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>

class SoundSystem {
public:
	SoundSystem(...) {}
	virtual ~SoundSystem() {}
	virtual void cleanup(...) {}
	virtual void newSource(...) {}
	virtual void newStreamingSource(...) {}
	virtual void play(...) {}
	virtual void setListenerOrientation(...) {}
	virtual void setListenerPosition(...) {}
	virtual void setPitch(...) {}
	virtual void setVolume(...) {}
	virtual void stop(...) {}
	virtual void pause(...) {}
	virtual void removeSource(...) {}
	virtual bool playing(...) { return false; }
};
#endif // SOUNDSYSTEM_H

#ifndef CODECJORBIS_H
#define CODECJORBIS_H

// ============================ EXTERNAL-LIBRARY STUB ============================
// Stands in for paulscode.sound.codecs.CodecJOrbis.
// This is NOT part of the decompiled Minecraft source -- it is an external Java
// library. Symbol-only: methods are no-ops / return defaults so the converted game
// code compiles and links; the real behaviour is the Xbox 360 platform layer
// (Direct3D 9 / XInput / file IO / sound).
// ==============================================================================
#include <string>
#include "URL.h"
#include "URLConnection.h"

class CodecJOrbis {
public:
	CodecJOrbis(...) {}
	virtual ~CodecJOrbis() {}
	virtual void initialize(...) {}
	virtual void cleanup(...) {}
	virtual void setAudioFormat(...) {}
	virtual void reverseByteOrder(...) {}
	virtual void* read(...) { return 0; }
	virtual void* readAll(...) { return 0; }
	virtual void* getAudioFormat(...) { return 0; }
	virtual bool initialized(...) { return false; }
	virtual bool endOfStream(...) { return false; }
	URL* url;
	URLConnection urlConnection;
};
#endif // CODECJORBIS_H

#ifndef CODECJORBIS_H
#define CODECJORBIS_H
// PORT stub: paulscode.sound.codecs.CodecJOrbis (Ogg Vorbis decoder). The 360
// build uses XACT (XactAudio) for all audio, so the paulscode codec chain is dead
// code -- this base just lets CodecMus (extends CodecJOrbis) compile.

class CodecJOrbis {
public:
	CodecJOrbis() {}
	virtual ~CodecJOrbis() {}
};

#endif // CODECJORBIS_H

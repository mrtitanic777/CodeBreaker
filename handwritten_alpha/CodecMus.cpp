#include "stdafx.h"
// Auto-generated from CodecMus.java
// Target: Xbox 360 / C++03 / DirectX 9
//
// PORT: dead audio code. CodecMus extends CodecJOrbis (a paulscode Ogg Vorbis
// codec). The 360 build uses XACT for ALL audio, so this codec chain is never
// driven. The original openInputStream() wraps a URLConnection input stream in
// a MusInputStream (none of which exist on 360); stubbed to return NULL so the
// class just compiles.

#include "CodecMus.h"
#include "Common.h"
#include "InputStream.h"

CodecMus::CodecMus()
{
}

CodecMus::~CodecMus() {
	// No dynamic resources
}

InputStream* CodecMus::openInputStream() {
		// PORT: 360 uses XACT; no Java InputStream / MusInputStream backing.
		return NULL;
}

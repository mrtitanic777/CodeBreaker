#ifndef CODECMUS_H
#define CODECMUS_H

// Auto-generated from CodecMus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CodecJOrbis.h"

class InputStream;

class CodecMus : public CodecJOrbis {
public:
	CodecMus();
	virtual ~CodecMus();

public: // was protected
	virtual InputStream* openInputStream();

};

#endif // CODECMUS_H
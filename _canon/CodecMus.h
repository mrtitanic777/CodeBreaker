#ifndef CODECMUS_H
#define CODECMUS_H

// Auto-generated from CodecMus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "CodecJOrbis.h"

class InputStream;

class CodecMus : public CodecJOrbis {
public:
	virtual ~CodecMus();

	virtual InputStream* openInputStream();

	static void initStatics();
};

#endif // CODECMUS_H
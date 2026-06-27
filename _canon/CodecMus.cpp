#include "stdafx.h"
#include "JavaRT.h"

// Auto-generated from CodecMus.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "CodecMus.h"
#include "CodecJOrbis.h"
#include "InputStream.h"
#include "MusInputStream.h"

CodecMus::~CodecMus()
{
}

InputStream* CodecMus::openInputStream()
{
	return new MusInputStream(this, this->url, this->urlConnection.getInputStream());
}

void CodecMus::initStatics()
{
}

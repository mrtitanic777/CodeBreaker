#ifndef MUSINPUTSTREAM_H
#define MUSINPUTSTREAM_H

// Auto-generated from MusInputStream.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
#include "InputStream.h"

class CodecMus;
class URL;

class MusInputStream : public InputStream {
public:
	MusInputStream(CodecMus* var1, URL* var2, InputStream* var3);
	virtual ~MusInputStream();

	int hash;
	InputStream* inputStream;
	JArray<unsigned char > buffer;
	CodecMus* codec;

	virtual int read();
	virtual int read(JArray<unsigned char > var1, int var2, int var3);

	static void initStatics();
};

#endif // MUSINPUTSTREAM_H
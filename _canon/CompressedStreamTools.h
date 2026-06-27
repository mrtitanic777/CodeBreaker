#ifndef COMPRESSEDSTREAMTOOLS_H
#define COMPRESSEDSTREAMTOOLS_H

// Auto-generated from CompressedStreamTools.java
// Target: Xbox 360 / C++03 / DirectX 9

#include "JavaString.h"
#include "JArray.h"
#include "JList.h"
class DataInput;
class DataOutput;
class InputStream;
class NBTTagCompound;
class OutputStream;

class CompressedStreamTools {
public:
	virtual ~CompressedStreamTools();

	static NBTTagCompound* readCompressed(InputStream* var0);
	static void writeGzippedCompoundToOutputStream(NBTTagCompound* var0, OutputStream* var1);
	static NBTTagCompound* decompress(JArray<unsigned char > var0);
	static JArray<unsigned char > compress(NBTTagCompound* var0);
	static NBTTagCompound* read(DataInput* var0);
	static void writeTag(NBTTagCompound* var0, DataOutput* var1);
};

#endif // COMPRESSEDSTREAMTOOLS_H